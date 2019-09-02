#include "os.h"
#include "os_cfg_app.h"
/*
**********************************************************
*                         数据域
**********************************************************
*/
CPU_STK  OSCfg_IdleTaskStk[OS_CFG_IDLE_TASK_STK_SIZE];


/*
**********************************************************
*                          常量
**********************************************************
*/
/* 空闲任务堆栈起始地址 */
CPU_STK  *const  OSCfg_IdleTaskStkBasePtr = \
	   (CPU_STK  *)&OSCfg_IdleTaskStk[0];

/* 空闲任务堆栈大小 */
CPU_STK_SIZE  const  OSCfg_IdleTaskStkSize = \
	   (CPU_STK_SIZE)OS_CFG_IDLE_TASK_STK_SIZE;
