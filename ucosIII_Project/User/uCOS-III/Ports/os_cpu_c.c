#include "os.h"
#include "ARMCM4.h"

/*
**操作系统任务堆栈初始化
*/
CPU_STK *OSTaskStkInit (OS_TASK_PTR   p_task, \
												void         *p_arg, \
												CPU_STK      *p_stk_base, \
												CPU_STK_SIZE  stk_size)
{
    CPU_STK *p_stk;
	
		p_stk = &p_stk_base[stk_size];
		/* 异常发生时自动保存的寄存器 */
		*--p_stk = (CPU_STK)0x01000000u;		/* xPSR的bit24必须置1(bit24为1使用Thumb指令，bit24为0不是Thumb指令，
																				** 由于Cortex—M4不支持ARM指令，所以24bit始终为1) 
																				*/
	  *--p_stk = (CPU_STK)p_task;					/* R15(PC)任务的入口地址 */
		*--p_stk = (CPU_STK)0x14141414u;    /* R14(LR)连接寄存器，用于存放函数返回程序地址 */
		*--p_stk = (CPU_STK)0x12121212u;    /* R12通用寄存器 */
		*--p_stk = (CPU_STK)0x03030303u;    /* R3 通用寄存器 */
	  *--p_stk = (CPU_STK)0x02020202u;    /* R2 通用寄存器 */
	  *--p_stk = (CPU_STK)0x01010101u;    /* R1 通用寄存器 */
	  *--p_stk = (CPU_STK)p_arg;          /* R0 通用寄存器：传递任务形参 */
	  /*异常发生时需手动保存的寄存器*/
	  *--p_stk = (CPU_STK)0x11111111u;    /* R11 */
	  *--p_stk = (CPU_STK)0x11111111u;    /* R10 */
	  *--p_stk = (CPU_STK)0x11111111u;    /* R9  */
	  *--p_stk = (CPU_STK)0x11111111u;    /* R8  */
	  *--p_stk = (CPU_STK)0x11111111u;    /* R7  */
	  *--p_stk = (CPU_STK)0x11111111u;    /* R6  */
		*--p_stk = (CPU_STK)0x11111111u;    /* R5  */
		*--p_stk = (CPU_STK)0x11111111u;    /* R4  */
	  
		return (p_stk);
}

/*
**SysTick初始化
*/
#if 0 /* 不用uCOS-III自带的 */
void  OS_CPU_SysTickInit(CPU_INT32U cnts)
{
    CPU_INT32U prio;
    
    /* 填写 SysTick 的重载计数值 */
    CPU_REG_NVIC_ST_RELOAD = cnts - 1u;

    /* 设置 SysTick 中断优先级 */
    prio = CPU_REG_NVIC_SHPRI3;
    prio &= DEF_BIT_FIELD(24, 0);
    prio |= DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO, 24);
    
    CPU_REG_NVIC_SHPRI13 = prio;

    /* 使能 SysTick 的时钟源和启动计数器 */
    CPU_REG32_NVIC_CTRL |= CPU_REG_NVIC_ST_STRL_CLKSOURCE|CPU_REG_NVIC_ST_CTRL_ENABLE;
		
		/* 使能 SysTick 的定时中断 */
		CPU_REG_NVIC_ST_CTRL |= CPU_REG_NVIC_ST_CTRL_TICKINT;
}

#else /* 直接使用头文件ARMCM4.h 里面现有的寄存器定义和函数来实现 */
void  OS_CPU_SysTickInit(CPU_INT32U ms)
{
	  /* 设置重装载寄存器的值 */
	  SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	
	  /* 配置中断优先级为最低 */
	  NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);//(1<<__NVIC_PRIO_BITS)-1为最低优先级，-2为更高一级的优先级，一次类推
	
	  /* 复位当前计数器的值 */
	  SysTick->VAL = 0;
	  
	  /* 选择时钟源、使能中断、使能计数器 */
	  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | \
	                  SysTick_CTRL_TICKINT_Msk   | \
	                  SysTick_CTRL_ENABLE_Msk;
}

#endif 

void SysTick_Handler(void)
{
	  OSTimeTick();
}
