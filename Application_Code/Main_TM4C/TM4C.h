/*
************************************************************************************************************************
*                                                TI-TM4C Flight Control
*                                               SCIE/Shanghai University
*                                              
* File    : TM4C.h
* By      : Bicbrv
* Note    : 
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
/*
**********************************************************************************************************
*UCOSIII���������ȼ��û�������ʹ��
*����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
*���ȼ�0���жϷ������������� OS_IntQTask()
*���ȼ�1��ʱ�ӽ������� OS_TickTask()
*���ȼ�2����ʱ���� OS_TmrTask()
*���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
*���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
**********************************************************************************************************
*/ 
#ifndef __CORE_MAIN_H
#define __CORE_MAIN_H

#include <core_common.h>
#include <board_includes.h>
#include <device_includes.h>
#include <math_includes.h>
#include <basicflight_includes.h>
#include <app_includes.h>
#include <OS_includes.h>
/*
************************************************************************************************************************
*                                               Start task
************************************************************************************************************************
*/
#define START_TASK_PRIO      3
#define START_STK_SIZE       512
extern OS_TCB	StartTaskTCB;
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
static void start_task(void *p_arg);

/*
************************************************************************************************************************
*                                               Flight init module
************************************************************************************************************************
*/
#define FLIGHT_INIT_TASK_PRIO      3
#define FLIGHT_INIT_STK_SIZE       512
extern OS_TCB	FlightINITTCB;
extern CPU_STK	FLIGHT_INIT_TASK_STK[FLIGHT_INIT_STK_SIZE];
static void flight_init_task(void *p_arg);
/*
========================================================================================================================
*                                               Defines
========================================================================================================================
*/
/* Caution!!! Don't set wings on your drone when committing change here!              */

#define MOTORRESET       false << 0u
#define NRF              true << 1u
/*
========================================================================================================================
*                                               Function Prototypes
========================================================================================================================
*/
static void FlightAPPInit(CPU_INT08U set);

#endif  //__MAIN_H__