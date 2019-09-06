#ifndef _OS_CPU_H_
#define _OS_CPU_H_
/*************************宏定义**************************/
#ifndef  NVIC_INT_CTRL
/* 中断控制及状态寄存器 SCB_ICSR */
#define  NVIC_INT_CTRL    *((CPU_REG32 *)0xE000ED04)
#endif

#ifndef  NVIC_PENDSVSET
/* 触发PendSV异常的值 Bit28: PENDSVSET */
#define  NVIC_PENDSVSET   0x10000000
#endif

/* 触发PendSV异常 */
#define  OS_TASK_SW()     NVIC_INT_CTRL = NVIC_PENDSVSET
/* 触发PendSV异常 */
#define  OSIntCtxSw()     NVIC_INT_CTRL = NVIC_PENDSVSET

/*
**********************************************************
*                       时间戳配置
**********************************************************
*/
/* 使能时间戳，在os_cfg.h 头文件中使用 */
#define  OS_CFG_TS_EN    				1u

#if      OS_CFG_TS_EN == 1u
#define  OS_TS_GET()						(CPU_TS)CPU_TS_TmrRd()
#else
#define  OS_TS_GET()            (CPU_TS)0u
#endif


/***********************宏定义end*************************/


/************************函数声明*************************/
void OSStartHighRdy(void); /* 在os_cpu_a.s中实现 */
void PendSV_Handler(void); /* 在os_cpu_a.s中实现 */
/**********************函数声明end************************/

#endif
