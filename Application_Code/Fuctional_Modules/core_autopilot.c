#include "core_autopilot.h"

float auto_throttle=0, error_throttle=0;
uint32 auto_throttle_max=68;
float control_y_out, control_x_out;
uint8 land_flag = 0;

int16 goto_count = 0;
uint16 bias_time_count = 0;
uint8 stablization_mode = 0;

void auto_test_flight_task(void *p_arg)
{
  OS_ERR err;	
//  CPU_SR_ALLOC();
  p_arg = p_arg;
  uint16 rxlen = 0;
  
  while(DEF_TRUE)
  {
    
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    
    if(UART1_RX_STA&0X8000)
    {
      rxlen = UART1_RX_STA&0X3FFF;
      UART1_RX_BUF[rxlen] = NULL;       
      
      if(UART1_RX_BUF[0] == '!')
      {  
        switch(UART1_RX_BUF[1])
        {
        case 'U':
          auto_throttle+=2;
          break;
          
        case 'D':
          auto_throttle-=2;
          break;
          
        case 'L':
          control_y_out-=2;
          break;
          
        case 'R':
          control_y_out+=2;
          break;
          
        case 'F':
          control_x_out+=2;
          break;
          
        case 'B':
          control_x_out-=2;
          break;          
          
        default:
          break;
        }
        UART1SendString("ok!\r\n");
      }
      
      if(UART1_RX_BUF[0] == '@')
        OSTaskSemPost(&AUTOtakeoff, OS_OPT_POST_NO_SCHED, &err);
      if(UART1_RX_BUF[0] == '#' && UART1_RX_BUF[1] == '#')
        OSTaskSemPost(&AUTOlanding, OS_OPT_POST_NO_SCHED, &err);
      
      if(auto_throttle<0)
        auto_throttle=0; 
      else if(auto_throttle>auto_throttle_max)
        auto_throttle-=2;
      
//      UART1_RX_STA = 0;
    }	
    UART1_RX_STA = 0;  
  }
}

void auto_takeoff_task(void *p_arg)
{
  OS_ERR err;	
//  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  uint32 set_throttle = 46;
  
//  OSTimeDlyHMSM(0,0,10,0,OS_OPT_TIME_HMSM_STRICT,&err);  
  while(DEF_TRUE)
  {
//    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("ok_take_off!\r\n");
//    set_throttle = atoi((char*)&UART1_RX_BUF[1]);
    OSTimeDlyHMSM(0,0,10,0,OS_OPT_TIME_HMSM_STRICT,&err);
//    if(flightStatus.Armed != FLIGHTSTATUS_ARMED_ARMED)
//      OSTaskDel(&AUTOtakeoff, &err);
    stablization_mode = 0;
    control_x_out = 2;
    while(auto_throttle < set_throttle && (flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED))
    {

      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle+=0.6;
//      control_x_out+=0.03;
    }
//    OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);    
    control_x_out = -2;
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);     
    control_x_out = 0;
    stablization_mode = 1;
    
    if(auto_throttle >= set_throttle)
    {  
//      task_flag = 1;
//      OSTimeDlyHMSM(0,0,20,0,OS_OPT_TIME_HMSM_STRICT,&err);      
      OSTaskSemPost(&AUTOgoto, OS_OPT_POST_NO_SCHED, &err);
//      OSTaskSemPost(&AUTOlanding, OS_OPT_POST_NO_SCHED, &err);
//      OSTaskSuspend (&AUTOtakeoff, &err);
      OSTaskDel(&AUTOtakeoff, &err);
    }
//    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);     
   
  }
}

void auto_landing_task(void *p_arg)
{
  OS_ERR err;	
//  CPU_SR_ALLOC();
  p_arg = p_arg;
  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("ok!\r\n");
    while(auto_throttle > 0)
    {
      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle-=0.5;
      if(ks103_distance<150)
      {
        auto_throttle=0;
        IMU_ext_flag=1;        
      }      
    }    
  }
}

void auto_goto_task(void *p_arg)
{
  OS_ERR err;	
//  CPU_SR_ALLOC();
  p_arg = p_arg;
//  uint16 goto_count = 0;
  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
//    UART1SendString("auto goto!\r\n");
//    while(goto_count<4)
//    {
//      control_x_out+=1;
//      OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//      control_x_out-=1;
//      OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err); 
//      goto_count++;
//    }
//    OSTaskSemPost(&AUTOlanding, OS_OPT_POST_NO_SCHED, &err); 
//    OSTaskSuspend (&AUTOgoto, &err);      
//    stablization_mode = 0;    
    
//    while(goto_count<5)
//    {
//      control_x_out+=1;
//      OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
//      control_x_out-=1;
//      OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err); 
//      goto_count++;
//    }

    goto_count = 0;
    stablization_mode = 1; 
    
    while(goto_count<50)
    {
      OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      if(((fabs(pic_x_cm)<90) && (fabs(pic_x_cm)>0)) && (fabs(pic_y_cm)<90 && (fabs(pic_y_cm)>0)))  
        goto_count++;
      else
        goto_count--;
      
      if(goto_count <= 0)
        goto_count = 0;
    }
    OSTaskSemPost(&AUTOlanding, OS_OPT_POST_NO_SCHED, &err);
    OSTaskDel(&AUTOgoto, &err);    
  }
}
          
/*
========================================================================================================================
*                                               TASK 1
========================================================================================================================
*/
void auto_takeoff_t1_task(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;
  p_arg = p_arg;
  
  uint32 set_throttle = 46;
 
  OSSemPend(&TAKEOFF_SIG,
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err);
  
  
  while(DEF_TRUE)
  {
    UART1SendString("takeoff!\r\n");

    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);

    while(auto_throttle < set_throttle && (flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED))
    {

      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle+=0.6;

    }

    if(auto_throttle >= set_throttle)
    {  
      OSTaskSemPost(&AUTOgoto_T1, OS_OPT_POST_NO_SCHED, &err);
      OSTaskDel(&AUTOtakeoff_T1, &err);
    }   
  }
}

void auto_goto_t1_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  
  uint16 bias_time_count = 0;
  uint16 total_time_count = 0;

  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("auto goto!\r\n");

    goto_count = 0;
    stablization_mode = 1; 
    
    while(DEF_TRUE)
    {
      OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      if(((fabs(pic_x_cm)<90) && (fabs(pic_x_cm)>0)) && (fabs(pic_y_cm)<90 && (fabs(pic_y_cm)>0)))  
      {  
        goto_count++;
        bias_time_count=0;
      }
      else
      {
        goto_count-=0.5;
        bias_time_count++;
      }
      
      if(goto_count <= 0)
        goto_count = 0;
      
      total_time_count++;
      
      if(goto_count>50 || bias_time_count>50 || total_time_count>300)
        break;
      
    }
    OSTaskSemPost(&AUTOlanding_T1, OS_OPT_POST_NO_SCHED, &err);
    OSTaskDel(&AUTOgoto_T1, &err);    
  }
}

void auto_landing_t1_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("landing!\r\n");
    while(auto_throttle > 0)
    {
      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle-=0.5;
      if(ks103_distance<150)
      {
        auto_throttle=0;
        IMU_ext_flag=1;
        OSTaskDel(&AUTOlanding_T1, &err);        
      }      
    }    
  }
}

/*
========================================================================================================================
*                                               TASK 2
========================================================================================================================
*/
void auto_takeoff_t2_task(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;
  p_arg = p_arg;
  
  uint32 set_throttle = 46;
 
  OSSemPend(&TAKEOFF_SIG,
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err);
  
  
  while(DEF_TRUE)
  {
    UART1SendString("takeoff!\r\n");

    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);

    stablization_mode = 0;
    control_x_out = 3;
    
    
    while(auto_throttle < set_throttle && (flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED))
    {

      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle+=0.6;

    }
    
    control_x_out = -3;
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);     
    control_x_out = 0;
    stablization_mode = 1;
    
    if(auto_throttle >= set_throttle)
    {  
      OSTaskSemPost(&AUTOgoto_T2, OS_OPT_POST_NO_SCHED, &err);
      OSTaskDel(&AUTOtakeoff_T2, &err);
    }   
  }
}

void auto_goto_t2_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  
  uint16 bias_time_count = 0;
  uint16 total_time_count = 0;

  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("auto goto!\r\n");

    goto_count = 0;
    stablization_mode = 1; 
    
    while(DEF_TRUE)
    {
      OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      if(((fabs(pic_x_cm)<90) && (fabs(pic_x_cm)>0)) && (fabs(pic_y_cm)<90 && (fabs(pic_y_cm)>0)))  
      {  
        goto_count++;
        bias_time_count=0;
      }
      else
      {
        goto_count-=0.5;
        bias_time_count++;
      }
      
      if(goto_count <= 0)
        goto_count = 0;
      
      total_time_count++;
      
      if(goto_count>50 || bias_time_count>50 || total_time_count>300)
        break;
      
    }
    OSTaskSemPost(&AUTOlanding_T2, OS_OPT_POST_NO_SCHED, &err);
    OSTaskDel(&AUTOgoto_T2, &err);    
  }
}

void auto_landing_t2_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("landing!\r\n");
    pic_y_cm += 80;
    OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);    
    while(auto_throttle > 0)
    {
      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle-=0.5;
      if(ks103_distance<150)
      {
        auto_throttle=0;
        IMU_ext_flag=1;
        OSTaskDel(&AUTOlanding_T2, &err);        
      }      
    }    
  }
}

/*
========================================================================================================================
*                                               TASK 3
========================================================================================================================
*/
void auto_takeoff_t3_task(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;
  p_arg = p_arg;
  
  uint32 set_throttle = 46;
 
  OSSemPend(&TAKEOFF_SIG,
            0,
            OS_OPT_PEND_BLOCKING,
            &ts,
            &err);
  
  
  while(DEF_TRUE)
  {
    UART1SendString("takeoff!\r\n");

    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);

    stablization_mode = 0;
    control_x_out = 3;
    
    
    while(auto_throttle < set_throttle && (flightStatus.Armed == FLIGHTSTATUS_ARMED_ARMED))
    {

      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle+=0.6;

    }
    
    control_x_out = -3;
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);     
    control_x_out = 0;
    stablization_mode = 1;
    
    if(auto_throttle >= set_throttle)
    {  
      OSTaskSemPost(&AUTOgoto_T3, OS_OPT_POST_NO_SCHED, &err);
      OSTaskDel(&AUTOtakeoff_T3, &err);
    }   
  }
}

void auto_goto_t3_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  
//  uint16 bias_time_count = 0;
  uint16 total_time_count = 0;

  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("auto goto!\r\n");

    stablization_mode = 1; 
    

    while(DEF_TRUE)
    {
      OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
      if(!land_flag)
      {
        if(((fabs(pic_x_cm)<90) && (fabs(pic_x_cm)>0)) && (fabs(pic_y_cm)<90 && (fabs(pic_y_cm)>0)))  
        {  
          bias_time_count=0;
        }
        else
        {
          bias_time_count++;
        }
        
        total_time_count++;
        
        if( bias_time_count>50 || total_time_count>600)
          break;
      }
      else if(land_flag)
      {
        OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
        break;
      }
      
    }
    OSTaskSemPost(&AUTOlanding_T3, OS_OPT_POST_NO_SCHED, &err);
    OSTaskDel(&AUTOgoto_T3, &err);    
  }
}

void auto_landing_t3_task(void *p_arg)
{
  OS_ERR err;	
  p_arg = p_arg;
  while(DEF_TRUE)
  {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
    UART1SendString("landing!\r\n");
    pic_y_cm += 80;
    OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);    
    while(auto_throttle > 0)
    {
      OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);       
      auto_throttle-=0.5;
      if(ks103_distance<150)
      {
        auto_throttle=0;
        IMU_ext_flag=1;
        OSTaskDel(&AUTOlanding_T3, &err);        
      }      
    }    
  }
}
