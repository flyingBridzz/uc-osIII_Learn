#ifndef _OS_CPU_H_
#define _OS_CPU_H_
/*************************�궨��**************************/
#ifndef  NVIC_INT_CTRL
/* �жϿ��Ƽ�״̬�Ĵ��� SCB_ICSR */
#define  NVIC_INT_CTRL    *((CPU_REG32 *)0xE000ED04)
#endif

#ifndef  NVIC_PENDSVSET
/* ����PendSV�쳣��ֵ Bit28: PENDSVSET */
#define  NVIC_PENDSVSET   0x10000000
#endif

/* ����PendSV�쳣 */
#define  OS_TASK_SW()     NVIC_INT_CTRL = NVIC_PENDSVSET
/* ����PendSV�쳣 */
#define  OSIntCtxSw()     NVIC_INT_CTRL = NVIC_PENDSVSET

/*
**********************************************************
*                       ʱ�������
**********************************************************
*/
/* ʹ��ʱ�������os_cfg.h ͷ�ļ���ʹ�� */
#define  OS_CFG_TS_EN    				1u

#if      OS_CFG_TS_EN == 1u
#define  OS_TS_GET()						(CPU_TS)CPU_TS_TmrRd()
#else
#define  OS_TS_GET()            (CPU_TS)0u
#endif


/***********************�궨��end*************************/


/************************��������*************************/
void OSStartHighRdy(void); /* ��os_cpu_a.s��ʵ�� */
void PendSV_Handler(void); /* ��os_cpu_a.s��ʵ�� */
/**********************��������end************************/

#endif
