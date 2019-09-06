#ifndef _CPU_H_
#define _CPU_H_

#include "cpu_def.h"

typedef    unsigned short    CPU_INT16U;
typedef    unsigned int      CPU_INT32U;
typedef    unsigned char     CPU_INT08U;
	
typedef    CPU_INT32U				 CPU_ADDR;
typedef    CPU_INT32U        CPU_SR;
	
/*��ջ���������ض���*/
typedef    CPU_INT32U				 CPU_STK;
typedef    CPU_ADDR					 CPU_STK_SIZE;

typedef    volatile CPU_INT32U    CPU_REG32;

/* CPU�ֳ����� */
#define    CPU_CFG_ADDR_SIZE          CPU_WORD_SIZE_32
#define    CPU_CFG_DATA_SIZE          CPU_WORD_SIZE_32
#define    CPU_CFG_DATA_SIZE_MAX      CPU_WORD_SIZE_64
typedef    CPU_INT32U					  			CPU_DATA;

/*****************����cpu_a.asm�еĺ���******************/
void CPU_IntDis(void);
void CPU_IntEn(void);

CPU_SR CPU_SR_Save(void);
void CPU_SR_Restore(CPU_SR cpu_sr);

#define  CPU_CFG_LEAD_ZEROS_ASM_PRESENT
CPU_DATA CPU_CntLeadZeros (CPU_DATA val);//cpu_a.asm �� cpu_core.c�ж��ж���
/********************��������end*************************/

#endif	/*_CPU_H_*/
