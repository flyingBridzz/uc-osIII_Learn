#ifndef _OS_H_
#define _OS_H_

#include "cpu.h"
#include "cpu_core.h"
#include "os_cfg.h"
#include "os_type.h"
#include "os_cpu.h"

#ifdef    OS_GLOBALS
#define   OS_EXT
#else
#define   OS_EXT    extern
#endif   /* OS_GLOBALS */

OS_EXT    OS_PRIO  OSPrioCur;        /* 当前优先级 */
OS_EXT    OS_PRIO  OSPrioHighRdy;    /* 最高优先级 */

#define  OS_PRIO_TBL_SIZE    ((OS_CFG_PRIO_MAX - 1u)/(DEF_INT_CPU_NBR_BITS) + 1u)

#define  OS_PRIO_INIT        (OS_PRIO)(OS_CFG_PRIO_MAX)


/*
========================================================================================================================
*                                                      任务的状态
========================================================================================================================
*/
#define  OS_STATE_OS_STOPPED                    (OS_STATE)(0u)
#define  OS_STATE_OS_RUNNING                    (OS_STATE)(1u)

                                                                /* ------------------- TASK STATES ------------------ */
#define  OS_TASK_STATE_BIT_DLY               (OS_STATE)(0x01u)  /*   /-------- SUSPENDED bit                          */
                                                                /*   |                                                */
#define  OS_TASK_STATE_BIT_PEND              (OS_STATE)(0x02u)  /*   | /-----  PEND      bit                          */
                                                                /*   | |                                              */
#define  OS_TASK_STATE_BIT_SUSPENDED         (OS_STATE)(0x04u)  /*   | | /---  Delayed/Timeout bit                    */
                                                                /*   | | |                                            */
                                                                /*   V V V                                            */

#define  OS_TASK_STATE_RDY                    (OS_STATE)(  0u)  /*   0 0 0     Ready                                  */
#define  OS_TASK_STATE_DLY                    (OS_STATE)(  1u)  /*   0 0 1     Delayed or Timeout                     */
#define  OS_TASK_STATE_PEND                   (OS_STATE)(  2u)  /*   0 1 0     Pend                                   */
#define  OS_TASK_STATE_PEND_TIMEOUT           (OS_STATE)(  3u)  /*   0 1 1     Pend + Timeout                         */
#define  OS_TASK_STATE_SUSPENDED              (OS_STATE)(  4u)  /*   1 0 0     Suspended                              */
#define  OS_TASK_STATE_DLY_SUSPENDED          (OS_STATE)(  5u)  /*   1 0 1     Suspended + Delayed or Timeout         */
#define  OS_TASK_STATE_PEND_SUSPENDED         (OS_STATE)(  6u)  /*   1 1 0     Suspended + Pend                       */
#define  OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED (OS_STATE)(  7u)  /*   1 1 1     Suspended + Pend + Timeout             */
#define  OS_TASK_STATE_DEL                    (OS_STATE)(255u)


/*
*****************************************************
*                    临界段处理
*****************************************************
*/
#define  OS_CRITICAL_ENTER()                      CPU_CRITICAL_ENTER()

#define  OS_CRITICAL_ENTER_CPU_CRITICAL_EXIT() 

#define  OS_CRITICAL_EXIT()                       CPU_CRITICAL_EXIT()

#define  OS_CRITICAL_EXIT_NO_SCHED()              CPU_CRITICAL_EXIT()


/*******************typedef定义*********************/
typedef    void(*OS_TASK_PTR)(void *P_arg);

typedef  struct  os_tick_spoke       OS_TICK_SPOKE;

typedef    struct os_tcb    OS_TCB;
struct os_tcb{
		CPU_STK       *StkPtr;
	  CPU_STK_SIZE  StkSize;
	  
	  /* 任务延时周期个数 */
	  OS_TICK       TaskDelayTicks;
	
	  /* 任务优先级 */
	  OS_PRIO       Prio;
	
	  /* 就绪列表双向链表的下一个指针 */
	  OS_TCB        *NextPtr;
	  /* 就绪列表双向链表的前一个指针 */
	  OS_TCB        *PrevPtr;
	
	  /* 时基列表相关字段 */
	  OS_TCB        *TickNextPtr;
	  OS_TCB        *TickPrevPtr;
	  OS_TICK_SPOKE *TickSpokePtr;
	  
	  OS_TICK       TickCtrMatch;
	  OS_TICK       TickRemain;
		
		/* 时间片相关字段 */
		OS_TICK       TimeQuanta;
		OS_TICK       TimeQuantaCtr;
		
		OS_STATE      TaskState;
		
#if OS_CFG_TASK_SUSPEND_EN > 0u
    /* 任务挂起函数 OSTaskSuspend() 计数器 */
		OS_NESTING_CTR  SuspendCtr;
#endif

};


typedef    struct os_rdy_list  OS_RDY_LIST;
struct os_rdy_list{
    OS_TCB          *HeadPtr;
	  OS_TCB          *TailPtr;
	  OS_OBJ_QTY      NbrEntries;
};


struct  os_tick_spoke{
    OS_TCB        *FirstPtr;
	  OS_OBJ_QTY     NbrEntries;
	  OS_OBJ_QTY     NbrEntriesMax;
};

typedef    enum os_err {
    OS_ERR_NONE                      =     0u,

    OS_ERR_A                         = 10000u,
    OS_ERR_ACCEPT_ISR                = 10001u,

    OS_ERR_B                         = 11000u,

    OS_ERR_C                         = 12000u,
    OS_ERR_CREATE_ISR                = 12001u,

    OS_ERR_D                         = 13000u,
    OS_ERR_DEL_ISR                   = 13001u,

    OS_ERR_E                         = 14000u,

    OS_ERR_F                         = 15000u,
    OS_ERR_FATAL_RETURN              = 15001u,

    OS_ERR_FLAG_GRP_DEPLETED         = 15101u,
    OS_ERR_FLAG_NOT_RDY              = 15102u,
    OS_ERR_FLAG_PEND_OPT             = 15103u,
    OS_ERR_FLUSH_ISR                 = 15104u,

    OS_ERR_G                         = 16000u,

    OS_ERR_H                         = 17000u,

    OS_ERR_I                         = 18000u,
    OS_ERR_ILLEGAL_CREATE_RUN_TIME   = 18001u,
    OS_ERR_INT_Q                     = 18002u,
    OS_ERR_INT_Q_FULL                = 18003u,
    OS_ERR_INT_Q_SIZE                = 18004u,
    OS_ERR_INT_Q_STK_INVALID         = 18005u,
    OS_ERR_INT_Q_STK_SIZE_INVALID    = 18006u,

    OS_ERR_J                         = 19000u,

    OS_ERR_K                         = 20000u,

    OS_ERR_L                         = 21000u,
    OS_ERR_LOCK_NESTING_OVF          = 21001u,

    OS_ERR_M                         = 22000u,

    OS_ERR_MEM_CREATE_ISR            = 22201u,
    OS_ERR_MEM_FULL                  = 22202u,
    OS_ERR_MEM_INVALID_P_ADDR        = 22203u,
    OS_ERR_MEM_INVALID_BLKS          = 22204u,
    OS_ERR_MEM_INVALID_PART          = 22205u,
    OS_ERR_MEM_INVALID_P_BLK         = 22206u,
    OS_ERR_MEM_INVALID_P_MEM         = 22207u,
    OS_ERR_MEM_INVALID_P_DATA        = 22208u,
    OS_ERR_MEM_INVALID_SIZE          = 22209u,
    OS_ERR_MEM_NO_FREE_BLKS          = 22210u,

    OS_ERR_MSG_POOL_EMPTY            = 22301u,
    OS_ERR_MSG_POOL_NULL_PTR         = 22302u,

    OS_ERR_MUTEX_NOT_OWNER           = 22401u,
    OS_ERR_MUTEX_OWNER               = 22402u,
    OS_ERR_MUTEX_NESTING             = 22403u,

    OS_ERR_N                         = 23000u,
    OS_ERR_NAME                      = 23001u,
    OS_ERR_NO_MORE_ID_AVAIL          = 23002u,

    OS_ERR_O                         = 24000u,
    OS_ERR_OBJ_CREATED               = 24001u,
    OS_ERR_OBJ_DEL                   = 24002u,
    OS_ERR_OBJ_PTR_NULL              = 24003u,
    OS_ERR_OBJ_TYPE                  = 24004u,

    OS_ERR_OPT_INVALID               = 24101u,

    OS_ERR_OS_NOT_RUNNING            = 24201u,
    OS_ERR_OS_RUNNING                = 24202u,

    OS_ERR_P                         = 25000u,
    OS_ERR_PEND_ABORT                = 25001u,
    OS_ERR_PEND_ABORT_ISR            = 25002u,
    OS_ERR_PEND_ABORT_NONE           = 25003u,
    OS_ERR_PEND_ABORT_SELF           = 25004u,
    OS_ERR_PEND_DEL                  = 25005u,
    OS_ERR_PEND_ISR                  = 25006u,
    OS_ERR_PEND_LOCKED               = 25007u,
    OS_ERR_PEND_WOULD_BLOCK          = 25008u,

    OS_ERR_POST_NULL_PTR             = 25101u,
    OS_ERR_POST_ISR                  = 25102u,

    OS_ERR_PRIO_EXIST                = 25201u,
    OS_ERR_PRIO                      = 25202u,
    OS_ERR_PRIO_INVALID              = 25203u,

    OS_ERR_PTR_INVALID               = 25301u,

    OS_ERR_Q                         = 26000u,
    OS_ERR_Q_FULL                    = 26001u,
    OS_ERR_Q_EMPTY                   = 26002u,
    OS_ERR_Q_MAX                     = 26003u,
    OS_ERR_Q_SIZE                    = 26004u,

    OS_ERR_R                         = 27000u,
    OS_ERR_REG_ID_INVALID            = 27001u,
    OS_ERR_ROUND_ROBIN_1             = 27002u,
    OS_ERR_ROUND_ROBIN_DISABLED      = 27003u,

    OS_ERR_S                         = 28000u,
    OS_ERR_SCHED_INVALID_TIME_SLICE  = 28001u,
    OS_ERR_SCHED_LOCK_ISR            = 28002u,
    OS_ERR_SCHED_LOCKED              = 28003u,
    OS_ERR_SCHED_NOT_LOCKED          = 28004u,
    OS_ERR_SCHED_UNLOCK_ISR          = 28005u,

    OS_ERR_SEM_OVF                   = 28101u,
    OS_ERR_SET_ISR                   = 28102u,

    OS_ERR_STAT_RESET_ISR            = 28201u,
    OS_ERR_STAT_PRIO_INVALID         = 28202u,
    OS_ERR_STAT_STK_INVALID          = 28203u,
    OS_ERR_STAT_STK_SIZE_INVALID     = 28204u,
    OS_ERR_STATE_INVALID             = 28205u,
    OS_ERR_STATUS_INVALID            = 28206u,
    OS_ERR_STK_INVALID               = 28207u,
    OS_ERR_STK_SIZE_INVALID          = 28208u,
    OS_ERR_STK_LIMIT_INVALID         = 28209u,

    OS_ERR_T                         = 29000u,
    OS_ERR_TASK_CHANGE_PRIO_ISR      = 29001u,
    OS_ERR_TASK_CREATE_ISR           = 29002u,
    OS_ERR_TASK_DEL                  = 29003u,
    OS_ERR_TASK_DEL_IDLE             = 29004u,
    OS_ERR_TASK_DEL_INVALID          = 29005u,
    OS_ERR_TASK_DEL_ISR              = 29006u,
    OS_ERR_TASK_INVALID              = 29007u,
    OS_ERR_TASK_NO_MORE_TCB          = 29008u,
    OS_ERR_TASK_NOT_DLY              = 29009u,
    OS_ERR_TASK_NOT_EXIST            = 29010u,
    OS_ERR_TASK_NOT_SUSPENDED        = 29011u,
    OS_ERR_TASK_OPT                  = 29012u,
    OS_ERR_TASK_RESUME_ISR           = 29013u,
    OS_ERR_TASK_RESUME_PRIO          = 29014u,
    OS_ERR_TASK_RESUME_SELF          = 29015u,
    OS_ERR_TASK_RUNNING              = 29016u,
    OS_ERR_TASK_STK_CHK_ISR          = 29017u,
    OS_ERR_TASK_SUSPENDED            = 29018u,
    OS_ERR_TASK_SUSPEND_IDLE         = 29019u,
    OS_ERR_TASK_SUSPEND_INT_HANDLER  = 29020u,
    OS_ERR_TASK_SUSPEND_ISR          = 29021u,
    OS_ERR_TASK_SUSPEND_PRIO         = 29022u,
    OS_ERR_TASK_WAITING              = 29023u,

    OS_ERR_TCB_INVALID               = 29101u,

    OS_ERR_TLS_ID_INVALID            = 29120u,
    OS_ERR_TLS_ISR                   = 29121u,
    OS_ERR_TLS_NO_MORE_AVAIL         = 29122u,
    OS_ERR_TLS_NOT_EN                = 29123u,
    OS_ERR_TLS_DESTRUCT_ASSIGNED     = 29124u,

    OS_ERR_TICK_PRIO_INVALID         = 29201u,
    OS_ERR_TICK_STK_INVALID          = 29202u,
    OS_ERR_TICK_STK_SIZE_INVALID     = 29203u,
    OS_ERR_TICK_WHEEL_SIZE           = 29204u,

    OS_ERR_TIME_DLY_ISR              = 29301u,
    OS_ERR_TIME_DLY_RESUME_ISR       = 29302u,
    OS_ERR_TIME_GET_ISR              = 29303u,
    OS_ERR_TIME_INVALID_HOURS        = 29304u,
    OS_ERR_TIME_INVALID_MINUTES      = 29305u,
    OS_ERR_TIME_INVALID_SECONDS      = 29306u,
    OS_ERR_TIME_INVALID_MILLISECONDS = 29307u,
    OS_ERR_TIME_NOT_DLY              = 29308u,
    OS_ERR_TIME_SET_ISR              = 29309u,
    OS_ERR_TIME_ZERO_DLY             = 29310u,

    OS_ERR_TIMEOUT                   = 29401u,

    OS_ERR_TMR_INACTIVE              = 29501u,
    OS_ERR_TMR_INVALID_DEST          = 29502u,
    OS_ERR_TMR_INVALID_DLY           = 29503u,
    OS_ERR_TMR_INVALID_PERIOD        = 29504u,
    OS_ERR_TMR_INVALID_STATE         = 29505u,
    OS_ERR_TMR_INVALID               = 29506u,
    OS_ERR_TMR_ISR                   = 29507u,
    OS_ERR_TMR_NO_CALLBACK           = 29508u,
    OS_ERR_TMR_NON_AVAIL             = 29509u,
    OS_ERR_TMR_PRIO_INVALID          = 29510u,
    OS_ERR_TMR_STK_INVALID           = 29511u,
    OS_ERR_TMR_STK_SIZE_INVALID      = 29512u,
    OS_ERR_TMR_STOPPED               = 29513u,

    OS_ERR_U                         = 30000u,

    OS_ERR_V                         = 31000u,

    OS_ERR_W                         = 32000u,

    OS_ERR_X                         = 33000u,

    OS_ERR_Y                         = 34000u,
    OS_ERR_YIELD_ISR                 = 34001u,

    OS_ERR_Z                         = 35000u
} OS_ERR;
/*****************typedef定义end********************/

/******************全局变量定义*********************/
OS_EXT  OS_RDY_LIST  OSRdyList[OS_CFG_PRIO_MAX];
OS_EXT  OS_TCB       *OSTCBCurPtr;
OS_EXT  OS_TCB       *OSTCBHighRdyPtr;
OS_EXT  OS_STATE     OSRunning;
OS_EXT  OS_TCB       OSIdleTaskTCB; //空闲任务TCB
OS_EXT  OS_IDLE_CTR  OSIdleTaskCtr; //空闲任务计数变量
OS_EXT  OS_TICK      OSTickCtr;     //Tick 计数器

/* 调度器锁嵌套计数器 */
OS_EXT            OS_NESTING_CTR         OSSchedLockNestingCtr;
/****************全局变量定义end********************/

/******************全局变量声明*********************/
/* 空闲任务堆栈起始地址 */
extern CPU_STK  *const  OSCfg_IdleTaskStkBasePtr;
/* 空闲任务堆栈大小 */
extern CPU_STK_SIZE  const  OSCfg_IdleTaskStkSize;
/* 优先级表声明 */
extern CPU_DATA  OSPrioTbl[OS_PRIO_TBL_SIZE];
/* 时基列表 */
extern OS_TICK_SPOKE  OSCfg_TickWheel[];
/* 时基列表大小 */
extern OS_OBJ_QTY  const  OSCfg_TickWheelSize;
/*****************全局变量声明end*******************/

/********************函数声明***********************/
CPU_STK *OSTaskStkInit (OS_TASK_PTR   p_task, \
												void         *p_arg, \
												CPU_STK      *p_stk_base, \
												CPU_STK_SIZE  stk_size);
												
void OSTaskCreate (OS_TCB        *p_tcb, \
									 OS_TASK_PTR    p_task, \
									 void          *p_arg, \
									 OS_PRIO       prio, \
									 CPU_STK       *p_stk_base, \
									 CPU_STK_SIZE  	stk_size, \
									 OS_TICK       time_quanta, \
									 OS_ERR        *p_err);
									 
void OS_RdyListInit(void);
void OSInit(OS_ERR *p_err);
void OSStart(OS_ERR *p_err);
void OS_CPU_SysTickInit(CPU_INT32U ms);
void OSTimeTick(void);
void OSSched(void);
void OS_IdleTaskInit(OS_ERR *p_err);
void OS_IdleTask(void *p_arg);
void OSTimeDly(OS_TICK dly);
									 
/* ------------- 就绪列表管理 ------------ */
void OS_RdyListInit            (void);
void OS_RdyListInsert          (OS_TCB       *p_tcb);
void OS_RdyListInsertHead      (OS_TCB       *p_tcb);
void OS_RdyListInsertTail      (OS_TCB       *p_tcb);
void OS_RdyListMoveHeadToTail  (OS_RDY_LIST  *p_rdy_list);
void OS_RdyListRemove          (OS_TCB       *p_tcb);
					
/* -------------- 优先级管理 ------------- */
void OS_PrioInit(void);
void OS_PrioInsert(OS_PRIO prio);
void OS_PrioRemove(OS_PRIO prio);
OS_PRIO OS_PrioGetHighest(void);

/* ------------- 时基列表管理 ------------ */
void OS_TickListInit(void);
void OS_TickListInsert(OS_TCB *p_tcb, OS_TICK time);
void OS_TickListRemove(OS_TCB *p_tcb);
void OS_TaskRdy(OS_TCB *p_tcb);
void OS_TickListUpdate(void);

/* ---------------- 时间片 --------------- */
void OS_SchedRoundRobin(OS_RDY_LIST *p_rdy_list);

/* ------------ 任务挂起与恢复 ----------- */
#if OS_CFG_TASK_SUSPEND_EN > 0u
void          OSTaskResume              (OS_TCB                *p_tcb,
                                         OS_ERR                *p_err);

void          OSTaskSuspend             (OS_TCB                *p_tcb,
                                         OS_ERR                *p_err);
#endif		
/*******************函数声明end*********************/
									 
#endif /* _OS_H_ */
