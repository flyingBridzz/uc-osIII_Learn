#include "os.h"

void OS_TaskInitTCB(OS_TCB *p_tcb)
{
	  p_tcb->StkPtr         = (CPU_STK    *)0;
	  p_tcb->StkSize        = (CPU_STK_SIZE)0u;
	  
	  p_tcb->TaskDelayTicks = (OS_TICK     )0u;
	
	  p_tcb->Prio           = (OS_PRIO     )OS_PRIO_INIT;
	
	  p_tcb->NextPtr        = (OS_TCB     *)0;
	  p_tcb->PrevPtr        = (OS_TCB     *)0;
}

void OSTaskCreate (OS_TCB        *p_tcb, \
									 OS_TASK_PTR    p_task, \
									 void          *p_arg, \
									 OS_PRIO       prio, \
									 CPU_STK       *p_stk_base, \
									 CPU_STK_SIZE  	stk_size, \
									 OS_TICK       time_quanta, \
									 OS_ERR        *p_err)
{
	  CPU_STK *p_sp;
	  CPU_SR_ALLOC();
	  
	  /* 初始化TCB为默认值 */
	  OS_TaskInitTCB(p_tcb);
	
	  /* 初始化堆栈 */
	  p_sp = OSTaskStkInit(p_task, \
												 p_arg, \
	                       p_stk_base, \
	                       stk_size);
	
	  p_tcb->Prio = prio;
	
	  p_tcb->StkPtr = p_sp;
	  p_tcb->StkSize = stk_size;
	
	  /* 时间片相关初始化 */
	  p_tcb->TimeQuanta    = time_quanta;
#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	  p_tcb->TimeQuantaCtr = time_quanta;
#endif
	
	  /* 进入临界段 */
	  OS_CRITICAL_ENTER();
	
	  /* 将任务添加到就绪列表 */
		OS_PrioInsert(p_tcb->Prio);
		OS_RdyListInsertTail(p_tcb);
		
		/* 退出临界段 */
		OS_CRITICAL_EXIT();
		
		*p_err = OS_ERR_NONE;
}

#if OS_CFG_TASK_SUSPEND_EN > 0u
void  OSTaskSuspend (OS_TCB *p_tcb, OS_ERR *p_err)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
		
		/* 是否挂起自己 */
		if(p_tcb == (OS_TCB *)0){
		    p_tcb = OSTCBCurPtr;
		}
		
		if(p_tcb == OSTCBCurPtr){
		    /* 如果调度器锁住则不能挂起自己 */
			  if(OSSchedLockNestingCtr > (OS_NESTING_CTR)0){
				    CPU_CRITICAL_EXIT();
					  *p_err = OS_ERR_SCHED_LOCKED;
					  return;
				}
		}
		
		*p_err = OS_ERR_NONE;
		
		/* 根据任务的状态来决定挂起的动作 */
		switch(p_tcb->TaskState){
			  case OS_TASK_STATE_RDY:
					  OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
				    p_tcb->TaskState  =  OS_TASK_STATE_SUSPENDED;
				    p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
				    OS_RdyListRemove(p_tcb);
				    OS_CRITICAL_EXIT_NO_SCHED();
				    break;
			
				case OS_TASK_STATE_DLY:
					  p_tcb->TaskState  =  OS_TASK_STATE_DLY_SUSPENDED;
				    p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
				    CPU_CRITICAL_EXIT();
				    break;
				
				case OS_TASK_STATE_PEND:
					  p_tcb->TaskState  =  OS_TASK_STATE_PEND_SUSPENDED;
				    p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
				    CPU_CRITICAL_EXIT();
				    break;
				
				case OS_TASK_STATE_PEND_TIMEOUT:
					  p_tcb->TaskState  =  OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED;
				    p_tcb->SuspendCtr = (OS_NESTING_CTR)1;
				    CPU_CRITICAL_EXIT();
				    break;
				
				case OS_TASK_STATE_SUSPENDED:
				case OS_TASK_STATE_DLY_SUSPENDED:
				case OS_TASK_STATE_PEND_SUSPENDED:
				case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
					  p_tcb->SuspendCtr++;
				    CPU_CRITICAL_EXIT();
				    break;
				
				default:
					  CPU_CRITICAL_EXIT();
				    *p_err = OS_ERR_STATE_INVALID;
				    break;
		}
		
		/* 任务切换 */
		OSSched();
}
#endif


#if OS_CFG_TASK_SUSPEND_EN > 0u
void OSTaskResume(OS_TCB  *p_tcb, OS_ERR *p_err)
{
	  CPU_SR_ALLOC();
	  
	  *p_err  = OS_ERR_NONE;
	  /* 根据任务的状态来决定挂起的动作 */
	  switch(p_tcb->TaskState){
			  case OS_TASK_STATE_RDY:
				case OS_TASK_STATE_DLY:
				case OS_TASK_STATE_PEND:
				case OS_TASK_STATE_PEND_TIMEOUT:
				    CPU_CRITICAL_EXIT();
				    *p_err = OS_ERR_TASK_NOT_SUSPENDED;
				    break;
				
				case OS_TASK_STATE_SUSPENDED:
					  OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT();
				    p_tcb->SuspendCtr--;
				    if(p_tcb->SuspendCtr == (OS_NESTING_CTR)0){
						    p_tcb->TaskState = OS_TASK_STATE_RDY;
							  OS_TaskRdy(p_tcb);
						}
						OS_CRITICAL_EXIT_NO_SCHED();
						break;
						
				case OS_TASK_STATE_DLY_SUSPENDED:
            p_tcb->SuspendCtr--;
            if(p_tcb->SuspendCtr == (OS_NESTING_CTR)0){
						    p_tcb->TaskState = OS_TASK_STATE_DLY;
						}
						CPU_CRITICAL_EXIT();
						break;
						
				case OS_TASK_STATE_PEND_SUSPENDED:
					  p_tcb->SuspendCtr--;
				    if(p_tcb->SuspendCtr == (OS_NESTING_CTR)0){
						    p_tcb->TaskState = OS_TASK_STATE_PEND;
						}
						CPU_CRITICAL_EXIT();
						break;
						
				case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
					  p_tcb->SuspendCtr--;
				    if(p_tcb->SuspendCtr == (OS_NESTING_CTR)0){
						    p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT;
						}
						CPU_CRITICAL_EXIT();
						break;
						
				default:
					  CPU_CRITICAL_EXIT();
				    *p_err = OS_ERR_STATE_INVALID;
				    return;
		}
		
		/* 任务切换 */
		OSSched();
}
#endif



