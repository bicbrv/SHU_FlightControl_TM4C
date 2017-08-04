#include "flight_routine.h"


void receive_date_check(void);

uint16 count0=0;
uint16 count1=0;
uint8 ADNS3080_Data_Buffer[7];
uint16 IMU_ext_flag=0;
uint16 ddd;
uint32 autostart_count=0;
uint8 Uart6Date;


//NRF�ж�
void PORTC_IRQHandler(void)
{
  OSIntEnter();
  
  uint32 ulStatus;
  ulStatus = GPIOIntStatus(GPIO_PORTC_BASE, true); // ��ȡ�ж�״̬              //GPIOIntTypeSet(NRF_IRQ_BASE,NRF_IRQ_PIN,GPIO_FALLING_EDGE) NRF_IRQ_PIN init as fall trigger interrupt
  GPIOIntClear(GPIO_PORTC_BASE, ulStatus);
  
  if(ulStatus&GPIO_PIN_5)                                                       //#define NRF_IRQ_PIN GPIO_PIN_5 Make sure NRF trigger is asserted
  {
    nrf_irq_flag=1;                                                             //set nrf_irq_flag
  }
  
  OSIntExit();  
}



void UART6_IRQHandler(void)
{
  OSIntEnter();
//  CPU_SR_ALLOC();
  
//  CPU_CRITICAL_ENTER();
  
  unsigned char Uart6Date; 
  uint32_t ui32Status;
  ui32Status = ROM_UARTIntStatus(UART6_BASE, true);
  ROM_UARTIntClear(UART6_BASE, ui32Status);
  
  while(ROM_UARTCharsAvail(UART6_BASE))
  {  
    Uart6Date = ROM_UARTCharGet(UART6_BASE);
    FLOW_MAVLINK(Uart6Date);
  }
  
//  CPU_CRITICAL_EXIT(); 
  
  OSIntExit(); 
}

void PIT_IRQHandler(void)
{
  OSIntEnter(); 
  
  OS_ERR err;
  
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  
  OSTaskSemPost(&FlightRoutineTCB, OS_OPT_POST_NONE, &err);
  
  OSIntExit();  
}

/*
========================================================================================================================
*                                               uc/OS Task
========================================================================================================================
*/

void flight_routine_task(void *p_arg)
{
  OS_ERR err;

  p_arg = p_arg;
  CPU_TS ts;
  CPU_SR_ALLOC();

  while(DEF_TRUE)
  {    
//    CPU_CRITICAL_ENTER();
    
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);

    t_tim0_cnt = TimerValueGet(TIMER0_BASE, TIMER_A);
//    OSSchedLock(&err);
//    mixing(flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED);
    
//    CPU_CRITICAL_ENTER();
    
//    OSMutexPend(&PID_adjust_MUTEX,
//                0,
//                OS_OPT_PEND_BLOCKING,
//                &ts,
//                &err);
    
    CPU_CRITICAL_ENTER();

    attsolving(); 
    
    stabilize();
    
    CPU_CRITICAL_EXIT();
    
//    OSMutexPost(&PID_adjust_MUTEX,
//                OS_OPT_POST_NONE,
//                &err);    
    
    OSMutexPend(&KS103_MUTEX,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
   
    hold(); 
    
    OSMutexPost(&KS103_MUTEX,
                OS_OPT_POST_NONE,
                &err);
//    CPU_CRITICAL_EXIT();
    
//    OSSchedUnlock(&err);

//    CPU_CRITICAL_ENTER();
   
   mixing(flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED);
    
   if(nrf_getcmd())
    {
      receive_date_check();
      nrf_sendstate();                                                              //��nrf��������,���ǿ����ڷ����ϼ��ϸ߶�,Data_int[8]���鳤����8
    }
   
    OSTaskSemPost(&UARTReportTCB, OS_OPT_POST_NONE, &err);
//    CPU_CRITICAL_EXIT();
    
    OSMutexPend(&FLOW_MUTEX,
              0,
              OS_OPT_PEND_BLOCKING,
              &ts,
              &err);
    
    command_handler();                                                            //::note::not fully research yet 
    
    OSMutexPost(&FLOW_MUTEX,
                OS_OPT_POST_NONE,
                &err);
        
    if(fabs(attitudeActual.Pitch)>40 || fabs(attitudeActual.Roll)>40)
      IMU_ext_flag=1;                                                             //::note:: How does IMU_ext_flag work?
    
    if(nrf_flag<=1)                                                               //if nrf is conneted & nrf data recieving process work, nrf_flag is always 400
      flightStatus.Armed=FLIGHTSTATUS_ARMED_DISARMED; 
    if(nrf_flag<=395) 
      LED0_OFF();                                                                 //DS2 turned on when nrf signal is well enough, 5*timestamp(maybe 12.5ms) unrecieved nrf signal may turn off DS2
    else 
      LED0_ON();
    
    mixing(flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED);
//    CPU_CRITICAL_EXIT();
//    OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);

    t_tim0_cnt = TimerValueGet(TIMER0_BASE, TIMER_A);
  }
}

void flight_routine_control_task(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;
  p_arg = p_arg;
//  CPU_SR_ALLOC();
  
  while(DEF_TRUE)
  {
    OSTimeDlyHMSM(0,0,0,25,OS_OPT_TIME_HMSM_STRICT,&err); 

    OSMutexPend(&FLOW_MUTEX,
              0,
              OS_OPT_PEND_BLOCKING,
              &ts,
              &err);
             
//    CPU_CRITICAL_ENTER();
    
//    OSSchedLock(&err);
    
    px4_data_fix();
    
   
    Control();
    
    
    
//    OSSchedUnlock(&err);    
    
//    CPU_CRITICAL_EXIT();
 
    
    OSMutexPost(&FLOW_MUTEX,
                OS_OPT_POST_NONE,
                &err);    

  }
}

void flight_routine_ks103_task(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;  
  p_arg = p_arg;  
//  CPU_SR_ALLOC();
  
  while(DEF_TRUE)
  {
//    OSSchedLock(&err);
    

    
    KS103_get_distance();                                                       //���

//    OSSchedUnlock(&err); 
    
    OSTimeDlyHMSM(0,0,0,113,OS_OPT_TIME_HMSM_STRICT,&err);  
    
//    CPU_CRITICAL_ENTER();    
    
//    OSSchedLock(&err);
    OSMutexPend(&KS103_MUTEX,
              0,
              OS_OPT_PEND_BLOCKING,
              &ts,
              &err);
   
    ks103_handler();
    
    
    OSMutexPost(&KS103_MUTEX,
                OS_OPT_POST_NONE,
                &err);      
//    OSSchedUnlock(&err);
    
//    CPU_CRITICAL_EXIT();
    
    OSTimeDlyHMSM(0,0,0,7,OS_OPT_TIME_HMSM_STRICT,&err);  
  }
}
/*
**********************************************************************************************************
**********************************************************************************************************
*/ 

