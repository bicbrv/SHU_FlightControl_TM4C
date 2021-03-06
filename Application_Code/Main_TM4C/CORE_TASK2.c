/*
************************************************************************************************************************
*                                                TI-TM4C Flight Control
*                                               SCIE/Shanghai University
*                                              
* File    : CORE_TASK2.c
* By      : Bicbrv
* Note    : task2 create
*
* TERMS OF USE:
* ---------------
*           We provide ALL the source code for your convenience and to help you 
*           keep developing our flight control firmware.  
*
*           Please help us continue to provide our project with the finest software available.
*           Your dedicated work is greatly appreciated. Feel free to ameliorate any 
*           part of our code without any restriction to pursue maximum performance.
*
************************************************************************************************************************
*/

#include "CORE_TASK2.h"

/*
************************************************************************************************************************
*                                               Auto takeoff task2
************************************************************************************************************************
*/
OS_TCB	AUTOtakeoff_T2;
CPU_STK	AUTO_TAKEOFF_T2_TASK_STK[AUTO_TAKEOFF_T2_TASK_SIZE];
/*
************************************************************************************************************************
*                                               Auto landing task2
************************************************************************************************************************
*/
OS_TCB	AUTOlanding_T2;
CPU_STK	AUTO_LANDIND_T2_TASK_STK[AUTO_LANDIND_T2_TASK_SIZE];
/*
************************************************************************************************************************
*                                               auto goto task2
************************************************************************************************************************
*/
OS_TCB	AUTOgoto_T2;
CPU_STK	AUTO_GOTO_T2_TASK_STK[AUTO_GOTO_T2_TASK_SIZE];
/*
========================================================================================================================
*                                               TASK 2
========================================================================================================================
*/

void TASK2_create(void)
{
  OS_ERR err; 
  OSTaskCreate((OS_TCB 	* )&FlightRoutineTCB,		
               (CPU_CHAR	* )"flight routine task", 		
               (OS_TASK_PTR  )flight_routine_task, 			
               (void	* )&COMPETITON_FLIGHT_MODE,					
               (OS_PRIO	  )FLIGHT_INIT_TASK_PRIO,     
               (CPU_STK    * )&FLIGHT_ROUTINE_TASK_STK[0],	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_STK_SIZE/10,	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  
  OSTaskCreate((OS_TCB 	* )&UARTAdjustTCB,		
               (CPU_CHAR	* )"uart adjust task", 		
               (OS_TASK_PTR  )uart_adjust_task, 			
               (void	* )0,					
               (OS_PRIO	  )UART_ADJUST_TASK_PRIO,     
               (CPU_STK    * )&UART_ADJUST_TASK_STK[0],	
               (CPU_STK_SIZE )UART_ADJUST_TASK_SIZE/10,	
               (CPU_STK_SIZE )UART_ADJUST_TASK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  
  OSTaskCreate((OS_TCB 	* )&AUTOtakeoff_T2,		
               (CPU_CHAR	* )"auto takeoff TASK2", 		
               (OS_TASK_PTR  )auto_takeoff_t2_task, 			
               (void	* )0,					
               (OS_PRIO	  )AUTO_TAKEOFF_T2_TASK_PRIO,     
               (CPU_STK    * )&AUTO_TAKEOFF_T2_TASK_STK[0],	
               (CPU_STK_SIZE )AUTO_TAKEOFF_T2_TASK_SIZE/10,	
               (CPU_STK_SIZE )AUTO_TAKEOFF_T2_TASK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  OSTaskCreate((OS_TCB 	* )&AUTOgoto_T2,		
               (CPU_CHAR	* )"auto goto TASK2", 		
               (OS_TASK_PTR  )auto_goto_t2_task, 			
               (void	* )0,					
               (OS_PRIO	  )AUTO_GOTO_T2_TASK_PRIO,     
               (CPU_STK    * )&AUTO_GOTO_T2_TASK_STK[0],	
               (CPU_STK_SIZE )AUTO_GOTO_T2_TASK_SIZE/10,	
               (CPU_STK_SIZE )AUTO_GOTO_T2_TASK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  OSTaskCreate((OS_TCB 	* )&AUTOlanding_T2,		
               (CPU_CHAR	* )"auto landing TASK2", 		
               (OS_TASK_PTR  )auto_landing_t2_task, 			
               (void	* )0,					
               (OS_PRIO	  )AUTO_LANDING_T2_TASK_PRIO,     
               (CPU_STK    * )&AUTO_LANDIND_T2_TASK_STK[0],	
               (CPU_STK_SIZE )AUTO_LANDIND_T2_TASK_SIZE/10,	
               (CPU_STK_SIZE )AUTO_LANDIND_T2_TASK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);

  OSSemCreate(&TAKEOFF_SIG, 
              "takeoff signal",
              (OS_SEM_CTR)0,
              &err);  

  
}