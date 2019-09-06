#define CPU_CORE_MODULE
#include "cpu_core.h"

/*
*****************************************************************
*                          寄存器定义
*****************************************************************
*/
#define  BSP_REG_DEM_CR                (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT            (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR             (*(CPU_REG32 *)0xE0042004)

/*
*****************************************************************
*                         寄存器位定义
*****************************************************************
*/
#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC               0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01             0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02             0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04             0xc0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                0xc0

#define  BSP_BIT_DEM_CR_TRCENA                        (1<<24)

#define  BSP_BIT_DWT_CR_CYCCNTENA                     (1<<0)

#ifndef  CPU_CFG_LEAD_ZEROS_ASM_PRESENT
static const CPU_INT08U  CPU_CntLeadZerosTbl[256] = {/*  索引        */
  8u,  7u,  6u,  6u,  5u,  5u,  5u,  5u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  /*   0x00 to 0x0F                   */
  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  /*   0x10 to 0x1F                   */
	2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x20 to 0x2F                   */
	2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x30 to 0x3F                   */
	1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x40 to 0x4F                   */
	1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x50 to 0x5F                   */
	1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x60 to 0x6F                   */
	1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x70 to 0x7F                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x80 to 0x8F                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x90 to 0x9F                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xA0 to 0xAF                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xB0 to 0xBF                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xC0 to 0xCF                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xD0 to 0xDF                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xE0 to 0xEF                   */
	0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u   /*   0xF0 to 0xFF                   */
};
#endif


/* 获取 CPU 的 HCLK 时钟
这个是跟硬件相关的，目前我们是软件仿真，我们暂时把跟硬件相关的代码屏蔽掉，
直接手动设置 CPU 的 HCLK 时钟 */
CPU_INT32U  BSP_CPU_ClkFreq(void)
{
#if 0
		RCC_ClocksTypeDef  rcc_clocks;
	  
		RCC_GetClocksFreq(&rcc_clocks);
	  return ((CPU_INT32U)rcc_clocks.HCLK_Frequency)
#else
	  CPU_INT32U CPU_HCLK;
	
		/* 目前软件仿真我们使用 25M 的系统时钟 */
	  CPU_HCLK = 25000000;
	
	  return CPU_HCLK;
#endif
}

/* 初始化CPU_TS_TmrFreq_Hz，这个就是系统的时钟，单位为Hz */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrFreqSet(CPU_TS_TMR_FREQ freq_hz)
{
	  CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif


/* 时间戳定时器初始化 */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrInit(void)
{
	  CPU_INT32U fclk_freq;
	
	  fclk_freq = BSP_CPU_ClkFreq();
	  
		/* 使能 DWT 外设 */
	  BSP_REG_DEM_CR |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA; 
	  /* DWT CYCCNT 寄存器计数清0 */
	  BSP_REG_DWT_CYCCNT = (CPU_INT32U)0u;
		/* 注意：当使用软件仿真全速运行的时候，会先停在这里，
							就好像在这里设置了一个断点一样，需要手动运行才能跳过，
	            当使用硬件仿真的时候却不会 */
	  /*使能 Cortex-M4 DWT CYCCNT寄存器 */
	  BSP_REG_DWT_CR |=(CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;
	
		CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


#if ((CPU_CFG_TS_EN == DEF_ENABLED) || \
     (CPU_CF_TS_TMR_EN == DEF_ENABLED))
static void  CPU_TS_Init(void)
{
	
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
    CPU_TS_TmrFreq_Hz = 0u;
    CPU_TS_TmrInit();
	
#endif
	
}
#endif


void CPU_Init(void)
{
#if ((CPU_CFG_TS_EN == DEF_ENABLED) || \
		 (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
		CPU_TS_Init();
#endif
	
}


#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR CPU_TS_TmrRd(void)
{
	  CPU_TS_TMR ts_tmr_cnts;
	
	
	  ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;
	
	  return (ts_tmr_cnts);
}
#endif

#ifndef  CPU_CFG_LEAD_ZEROS_ASM_PRESENT
CPU_DATA CPU_CntLeadZeros(CPU_DATA val)
{
	  CPU_DATA    nbr_lead_zeros;
	  CPU_INT08U  ix;
	  
	  /* 检查高十六位 */
	  if(val > 0x0000FFFFu){
			  /* 检查bits[31:24] */
			  if(val > 0x00FFFFFFu){
					  /* 获取bis[31:24]的值，并转换成8位 */
					  ix = (CPU_INT08U)(val >> 24u);
					  /* 查前导零个数表，找到最高优先级 */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+0u);
				}
				/* 检查bits[23:16] */
				else{
				    /* 获取bis[23:16]的值，并转换成8位 */
					  ix = (CPU_INT08U)(val >> 16u);
					  /* 查前导零个数表，找到最高优先级 */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+8u);
				}
		}
		/* 检查低十六位 */
		else{
			  /* 检查bits[15:08] */
			  if(val > 0x000000FFu){
				    /* 获取bis[15:08]的值，并转换成8位 */
					  ix = (CPU_INT08U)(val >> 8u);
					  /* 查前导零个数表，找到最高优先级 */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+16u);
				}
				/* 检查bits[07:00] */
				else{
				    /* 获取bis[07:00]的值，并转换成8位 */
					  ix = (CPU_INT08U)(val >> 0u);
					  /* 查前导零个数表，找到最高优先级 */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+24u);
				}
		}
		
		return (nbr_lead_zeros);
}
#endif
