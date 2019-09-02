#include "os.h"

void OS_RdyListInit(void)
{
	  OS_PRIO i;
	  OS_RDY_LIST *p_rdy_list;
	  
	  for( i=0u; i<OS_CFG_PRIO_MAX; i++){
		    p_rdy_list = &OSRdyList[i];
			  p_rdy_list->HeadPtr = (OS_TCB *)0;
			  p_rdy_list->TailPtr = (OS_TCB *)0;
		}
}

void OSInit(OS_ERR *p_err)
{
	  OSRunning = OS_STATE_OS_STOPPED;
	  
	  OSTCBCurPtr = (OS_TCB *)0;
	  OSTCBHighRdyPtr = (OS_TCB *)0;
	
	  OS_RdyListInit();
	  
	  /* 初始化空闲任务 */
	  OS_IdleTaskInit(p_err);
	  if(*p_err != OS_ERR_NONE){
		    return;
		}
}

/*任务切换，实际就是触发 PendSV 异常，然后再PendSV异常中进行上下文切换 */
void OSSched(void)
{
#if 0
	  if(OSTCBCurPtr == OSRdyList[0].HeadPtr){
		    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
		}else{
		    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		}
#endif
		
	  /* 如果当前任务是空闲任务，那么就去尝试执行任务1或者任务2，
       看看他们的延时时间是否结束，如果任务的延时时间均没有到期，
       那就返回继续执行空闲任务	*/
	  if(OSTCBCurPtr == &OSIdleTaskTCB){
		    if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0){
				    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
				}else if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0){
				    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
				}else{
				    /* 任务延时均没有到期则返回，继续执行空闲任务 */
					  return;
				}
		}else{
		    /*如果是 task1 或者 task2 的话，检查下另外一个任务，
			    如果另外的任务不在延时中，就切换到该任务
			    否则，判断下当前任务是否应该进入延时状态，
			    如果是的话，就切换到空闲任务，否则就不进行任何切换 */
		    if(OSTCBCurPtr == OSRdyList[0].HeadPtr){
				    if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0){
						    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
						}else if(OSTCBCurPtr->TaskDelayTicks != 0){
						    OSTCBHighRdyPtr = &OSIdleTaskTCB;
						}else{
						    /* 返回原任务 */
							  return;
						}
				}else if(OSTCBCurPtr == OSRdyList[1].HeadPtr){
				    if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0){
						    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
						}else if(OSTCBCurPtr->TaskDelayTicks != 0){
						    OSTCBHighRdyPtr = &OSIdleTaskTCB;
						}else{
						    /* 返回原任务 */
							  return;
						}
				}
		}
		/*切换任务*/
		OS_TASK_SW();
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
	  /* 初始化空闲任务计数器 */
	  OSIdleTaskCtr = (OS_IDLE_CTR)0;
	  
	  /* 创建空闲任务 */
	  OSTaskCreate((OS_TCB     *)&OSIdleTaskTCB, \
	               (OS_TASK_PTR )OS_IdleTask, \
	               (void       *)0, \
								 (CPU_STK    *)OSCfg_IdleTaskStkBasePtr, \
								 (CPU_STK_SIZE)OSCfg_IdleTaskStkSize, \
								 (OS_ERR     *)p_err );
}

void OSStart(OS_ERR *p_err)
{
	  if(OSRunning == OS_STATE_OS_STOPPED){
		    /* 手动配置任务1先运行 */
			  OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
			
			  /* 启动任务切换，不会返回 */
			  OSStartHighRdy();
			
			  /* 不会运行到这里，运行到这里表示发生了致命错误 */
			  *p_err = OS_ERR_FATAL_RETURN;
		}else{
		    *p_err = OS_STATE_OS_RUNNING;
		}
}

void OS_IdleTask(void *p_arg)
{
	  p_arg = p_arg;
	
	  /* 空闲任务什么都不做，只对全局变量OSIdleTaskCtr++ 操作 */
	  while(1){
	      OSIdleTaskCtr++;
  	}
}

