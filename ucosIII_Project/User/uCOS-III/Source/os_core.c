#include "os.h"

void OS_RdyListInit(void)
{
	  OS_PRIO i;
	  OS_RDY_LIST *p_rdy_list;
	  
	  for( i=0u; i<OS_CFG_PRIO_MAX; i++){
		    p_rdy_list = &OSRdyList[i];
			  p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
			  p_rdy_list->HeadPtr = (OS_TCB *)0;
			  p_rdy_list->TailPtr = (OS_TCB *)0;
		}
}

/*
*�ھ����б�β������һ��TCB��������������
*
*CASE 0:�����ǿյ�
*              OS_RDY_LIST
*              +--------------+
*              |   TailPtr    |-->
*              +--------------+
*              |   HeadPtr    |-->
*              +--------------+
*              | NbrEntries=0 |
*              +--------------+
*
*
*CASE 1:�������нڵ�
*              OS_RDY_LIST
*              +--------------+        OS_TCB
*              |   TailPtr    |--+---> +--------------+
*              +--------------+  |     |   NextPtr    |->0
*              |   HeadPtr    |--/     +--------------+
*              +--------------+     0<-|   PrevPtr    |
*              | NbrEntries=1 |        +--------------+
*              +--------------+        :              :
*                                      :              :
*                                      +--------------+
*
*
*              OS_RDY_LIST
*              +--------------+        
*              |   TailPtr    |--------------------------------------------------+
*              +--------------+        OS_TCB                  OS_TCB            |   OS_TCB
*              |   HeadPtr    |------->+--------------+        +--------------+  +-->+--------------+
*              +--------------+        |   NextPtr    |------->|   NextPtr    |......|   NextPtr    |->0
*              | NbrEntries=1 |        +--------------+        +--------------+      +--------------+
*              +--------------+     0<-|   PrevPtr    |<-------|   PrevPtr    |......|   PrevPtr    |
*                                      +--------------+        +--------------+      +--------------+
*                                      :              :        :              :      :              :
*                                      :              :        :              :      :              :
*                                      +--------------+        +--------------+      +--------------+
*
*/
void OS_RdyListInsertHead(OS_TCB *p_tcb)
{
	  OS_RDY_LIST  *p_rdy_list;
	  OS_TCB       *p_tcb2;
	  
	  /* ��ȡ������� */
	  p_rdy_list = &OSRdyList[p_tcb->Prio];
	  
	 /* CASE 0: �����ǿ����� */
	 if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0){
		    p_rdy_list->NbrEntries = (OS_OBJ_QTY)1;
			  p_tcb->NextPtr         = (OS_TCB   *)0;
			  p_tcb->PrevPtr         = (OS_TCB   *)0;
			  p_rdy_list->HeadPtr    = p_tcb;
			  p_rdy_list->TailPtr    = p_tcb; 
		}
	  /* CASE 1: �������нڵ� */
		else{
		    p_rdy_list->NbrEntries++;
			  p_tcb->PrevPtr         = (OS_TCB   *)0;
			  p_tcb2                 = p_rdy_list->HeadPtr;
			  p_tcb->NextPtr         = p_tcb2;
			  p_tcb2->PrevPtr        = p_tcb;
			  p_rdy_list->HeadPtr    = p_tcb;
		}
}

void OS_RdyListInsertTail(OS_TCB *p_tcb)
{
	  OS_RDY_LIST *p_rdy_list;
	  OS_TCB      *p_tcb2;
	
	  /* ��ȡ������� */
	  p_rdy_list = &OSRdyList[p_tcb->Prio];
	
	  /* CASE 0: �����ǿյ� */
	  if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0){
		    p_rdy_list->NbrEntries = (OS_OBJ_QTY)1;
			  p_tcb->NextPtr         = (OS_TCB   *)0;
			  p_tcb->PrevPtr         = (OS_TCB   *)0;
			  p_rdy_list->HeadPtr    = p_tcb;
			  p_rdy_list->TailPtr    = p_tcb; 
		}
		/* CASE1: �������нڵ� */
		else{
		    p_rdy_list->NbrEntries++;
			  p_tcb->NextPtr         = (OS_TCB   *)0;
			  p_tcb2                 = p_rdy_list->TailPtr;
			  p_tcb->PrevPtr         = p_tcb2;
			  p_tcb2->NextPtr        = p_tcb;
			  p_rdy_list->TailPtr    = p_tcb;
		}
}

void OS_RdyListInsert(OS_TCB *p_tcb)
{
	  /* �����ȼ����뵽���ȼ��� */
	  OS_PrioInsert(p_tcb->Prio);
	  
	  if(p_tcb->Prio == OSPrioCur){
		    /* ����ǵ�ǰ���ȼ�����뵽����β�� */
			  OS_RdyListInsertTail(p_tcb);
		}else{
		    /* ������뵽����ͷ�� */
			  OS_RdyListInsertHead(p_tcb);
		}
}

/*
*OS_RdyListMoveHeadToTail()������������ͷ���ƶ���β��
*���������������
*1������Ϊ�գ����¿���
*2������ֻ��һ���ڵ㣬Ҳ�����¿���
*3������ֻ�������ڵ�
*4���������������Ͻڵ�
*/
void OS_RdyListMoveHeadToTail(OS_RDY_LIST *p_rdy_list)
{
	  OS_TCB *p_tcb1;
	  OS_TCB *p_tcb2;
	  OS_TCB *p_tcb3;
	
	  switch(p_rdy_list->NbrEntries){
				case 0:
				case 1: break;
				
				case 2: p_tcb1              = p_rdy_list->HeadPtr;
								p_tcb2              = p_rdy_list->TailPtr;
								p_tcb1->PrevPtr     = p_tcb2;
								p_tcb1->NextPtr     = (OS_TCB *)0;
								p_tcb2->PrevPtr     = (OS_TCB *)0;
								p_tcb2->NextPtr     = p_tcb1;
								p_rdy_list->HeadPtr = p_tcb2;
								p_rdy_list->TailPtr = p_tcb1;
								break;
				 
				default:p_tcb1              = p_rdy_list->HeadPtr;
								p_tcb2              = p_rdy_list->TailPtr;
								p_tcb3              = p_tcb1->NextPtr;
								p_tcb3->PrevPtr     = (OS_TCB *)0;
								p_tcb1->NextPtr     = (OS_TCB *)0;
								p_tcb1->PrevPtr     = p_tcb2;
								p_tcb2->NextPtr     = p_tcb1;
								p_rdy_list->HeadPtr = p_tcb3;
								p_rdy_list->TailPtr = p_tcb1;
								break;
		}
}

/*
*OS_RdyListMoveHeadToTail()������������ͷ���ƶ���β��
*���������������
*1������Ϊ�գ����¿���
*2������ֻ��һ���ڵ㣬Ҳ�����¿���
*3������ֻ�����������Ͻڵ�
*/
void OS_RdyListRemove(OS_TCB *p_tcb)
{
	  OS_RDY_LIST *p_rdy_list;
	  OS_TCB      *p_tcb1;
	  OS_TCB      *p_tcb2;
	
	  p_rdy_list = &OSRdyList[p_tcb->Prio];
	  
	  /* ����Ҫɾ����TCB�ڵ��ǰһ���ͺ�һ���ڵ� */
	  p_tcb1     = p_tcb->PrevPtr;
	  p_tcb2     = p_tcb->NextPtr;
	
	  /* Ҫ�Ƴ���TCB�ڵ��������еĵ�һ���ڵ� */
	  if(p_tcb1 == (OS_TCB *)0)
		{
		    /* �Ҹ�������ֻ��һ���ڵ� */
			  if(p_tcb2 == (OS_TCB *)0)
				{
				    /* ���ڵ�ȫ����ʼ��Ϊ0 */
					  p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
					  p_rdy_list->HeadPtr    = (OS_TCB   *)0;
					  p_rdy_list->TailPtr    = (OS_TCB   *)0;
					  
					  /* ��������ȼ�������Ӧ��λ */
					  OS_PrioRemove(p_tcb->Prio);//ֻ��������ֻ��һ���ڵ�ʱ���Ż�ɾ�����ȼ�������Ӧ��λ
				}
				/* �������в�ֹһ���ڵ� */
				else{
				    /* �ڵ��1 */
					  p_rdy_list->NbrEntries--;
					  p_tcb2->PrevPtr        = (OS_TCB   *)0;
					  p_rdy_list->HeadPtr    = p_tcb2;
				}
		}
		/* Ҫɾ����TCB�ڵ㲻�������еĵ�һ���ڵ� */
		else{
	      p_rdy_list->NbrEntries--;
			  p_tcb1->NextPtr = p_tcb2;
			
			  /* ���Ҫɾ���Ľڵ����һ���ڵ���0����Ҫɾ���Ľڵ������һ���ڵ� */
			  if(p_tcb2 == (OS_TCB *)0){
				    p_rdy_list->TailPtr = p_tcb1;
				}else{
				    p_tcb2->PrevPtr     = p_tcb1;
				}
		}
		
		/* ��λ�پ�������ɾ����TCB�� PrevPtr �� NextPtr ������ָ�� */
		p_tcb->PrevPtr = (OS_TCB *)0;
		p_tcb->NextPtr = (OS_TCB *)0;
}

void OSInit(OS_ERR *p_err)
{
	  OSRunning = OS_STATE_OS_STOPPED;
	  
	  /* ��ʼ������ȫ��TCBָ�룬������TCB���������л� */
	  OSTCBCurPtr = (OS_TCB *)0;
	  OSTCBHighRdyPtr = (OS_TCB *)0;
	  
	  /* ��ʼ�����ȼ����� */
	  OSPrioCur     = (OS_PRIO)0;
	  OSPrioHighRdy = (OS_PRIO)0;
	
	  /* ��ʼ�����ȼ��� */
	  OS_PrioInit();
	  
	  OS_RdyListInit();
	  
	  /* ��ʼ���������� */
	  OS_IdleTaskInit(p_err);
	  if(*p_err != OS_ERR_NONE){
		    return;
		}
}


/*�����л���ʵ�ʾ��Ǵ��� PendSV �쳣��Ȼ����PendSV�쳣�н����������л� */
void OSSched(void)
{
#if 0
	  if(OSTCBCurPtr == OSRdyList[0].HeadPtr){
		    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
		}else{
		    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		}
#endif

#if 0	
	  /* �����ǰ�����ǿ���������ô��ȥ����ִ������1��������2��
       �������ǵ���ʱʱ���Ƿ����������������ʱʱ���û�е��ڣ�
       �Ǿͷ��ؼ���ִ�п�������	*/
	  if(OSTCBCurPtr == &OSIdleTaskTCB){
		    if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0){
				    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
				}else if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0){
				    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
				}else{
				    /* ������ʱ��û�е����򷵻أ�����ִ�п������� */
					  return;
				}
		}else{
		    /*����� task1 ���� task2 �Ļ������������һ������
			    ����������������ʱ�У����л���������
			    �����ж��µ�ǰ�����Ƿ�Ӧ�ý�����ʱ״̬��
			    ����ǵĻ������л����������񣬷���Ͳ������κ��л� */
		    if(OSTCBCurPtr == OSRdyList[0].HeadPtr){
				    if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0){
						    OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
						}else if(OSTCBCurPtr->TaskDelayTicks != 0){
						    OSTCBHighRdyPtr = &OSIdleTaskTCB;
						}else{
						    /* ����ԭ���� */
							  return;
						}
				}else if(OSTCBCurPtr == OSRdyList[1].HeadPtr){
				    if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0){
						    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
						}else if(OSTCBCurPtr->TaskDelayTicks != 0){
						    OSTCBHighRdyPtr = &OSIdleTaskTCB;
						}else{
						    /* ����ԭ���� */
							  return;
						}
				}
		}
		/*�л�����*/
		OS_TASK_SW();
#endif

    CPU_SR_ALLOC();
		
		/*�����ٽ��*/
		OS_CRITICAL_ENTER();
		
		/* ����������ȼ������� */
		OSPrioHighRdy   = OS_PrioGetHighest();
		OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
		
		/* ���������ȼ��������ǵ�ǰ������ֱ�ӷ��أ������������л� */
		if(OSTCBHighRdyPtr == OSTCBCurPtr){
		    /* �˳��ٽ�� */
			  OS_CRITICAL_EXIT();
			
			  return;
		}
		/* �˳��ٽ�� */
		OS_CRITICAL_EXIT();
		
		OS_TASK_SW();
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
	  /* ��ʼ��������������� */
	  OSIdleTaskCtr = (OS_IDLE_CTR)0;
	  
	  /* ������������ */
	  OSTaskCreate((OS_TCB     *)&OSIdleTaskTCB, \
	               (OS_TASK_PTR )OS_IdleTask, \
	               (void       *)0, \
								 (OS_PRIO)(OS_CFG_PRIO_MAX - 1u), \
								 (CPU_STK    *)OSCfg_IdleTaskStkBasePtr, \
								 (CPU_STK_SIZE)OSCfg_IdleTaskStkSize, \
								 (OS_ERR     *)p_err );
}

void OSStart(OS_ERR *p_err)
{
	  if(OSRunning == OS_STATE_OS_STOPPED){
#if 0
		    /* �ֶ���������1������ */
			  OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
#endif
			  /* Ѱ����ߵ����ȼ� */
			  OSPrioHighRdy   = OS_PrioGetHighest();
			  OSPrioCur       = OSPrioHighRdy;
			
			  /*�ҵ�������ȼ���TCB*/
			  OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
			  OSTCBCurPtr     = OSTCBHighRdyPtr;
			
			  /* ��� OS ��ʼ���� */
			  OSRunning       = OS_STATE_OS_RUNNING;
			  
			  /* ���������л������᷵�� */
			  OSStartHighRdy();
			
			  /* �������е�������е������ʾ�������������� */
			  *p_err = OS_ERR_FATAL_RETURN;
		}else{
		    *p_err = OS_STATE_OS_RUNNING;
		}
}

void OS_IdleTask(void *p_arg)
{
	  p_arg = p_arg;
	
	  /* ��������ʲô��������ֻ��ȫ�ֱ���OSIdleTaskCtr++ ���� */
	  while(1){
	      OSIdleTaskCtr++;
  	}
}

