#ifndef _CPU_H_
#define _CPU_H_

typedef    unsigned short    CPU_INT16U;
typedef    unsigned int      CPU_INT32U;
typedef    unsigned char     CPU_INT08U;
	
typedef    CPU_INT32U				 CPU_ADDR;
	
/*堆栈数据类型重定义*/
typedef    CPU_INT32U				 CPU_STK;
typedef    CPU_ADDR					 CPU_STK_SIZE;

typedef    volatile CPU_INT32U    CPU_REG32;

/*****************声明cpu_a.asm中的函数******************/
void CPU_IntDis(void);
void CPU_IntEn(void);
/********************函数声明end*************************/

#endif	/*_CPU_H_*/
