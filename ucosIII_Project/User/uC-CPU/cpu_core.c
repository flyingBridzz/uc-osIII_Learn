#define CPU_CORE_MODULE
#include "cpu_core.h"

/*
*****************************************************************
*                          �Ĵ�������
*****************************************************************
*/
#define  BSP_REG_DEM_CR                (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT            (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR             (*(CPU_REG32 *)0xE0042004)

/*
*****************************************************************
*                         �Ĵ���λ����
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
static const CPU_INT08U  CPU_CntLeadZerosTbl[256] = {/*  ����        */
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


/* ��ȡ CPU �� HCLK ʱ��
����Ǹ�Ӳ����صģ�Ŀǰ������������棬������ʱ�Ѹ�Ӳ����صĴ������ε���
ֱ���ֶ����� CPU �� HCLK ʱ�� */
CPU_INT32U  BSP_CPU_ClkFreq(void)
{
#if 0
		RCC_ClocksTypeDef  rcc_clocks;
	  
		RCC_GetClocksFreq(&rcc_clocks);
	  return ((CPU_INT32U)rcc_clocks.HCLK_Frequency)
#else
	  CPU_INT32U CPU_HCLK;
	
		/* Ŀǰ�����������ʹ�� 25M ��ϵͳʱ�� */
	  CPU_HCLK = 25000000;
	
	  return CPU_HCLK;
#endif
}

/* ��ʼ��CPU_TS_TmrFreq_Hz���������ϵͳ��ʱ�ӣ���λΪHz */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrFreqSet(CPU_TS_TMR_FREQ freq_hz)
{
	  CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif


/* ʱ�����ʱ����ʼ�� */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrInit(void)
{
	  CPU_INT32U fclk_freq;
	
	  fclk_freq = BSP_CPU_ClkFreq();
	  
		/* ʹ�� DWT ���� */
	  BSP_REG_DEM_CR |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA; 
	  /* DWT CYCCNT �Ĵ���������0 */
	  BSP_REG_DWT_CYCCNT = (CPU_INT32U)0u;
		/* ע�⣺��ʹ���������ȫ�����е�ʱ�򣬻���ͣ�����
							�ͺ���������������һ���ϵ�һ������Ҫ�ֶ����в���������
	            ��ʹ��Ӳ�������ʱ��ȴ���� */
	  /*ʹ�� Cortex-M4 DWT CYCCNT�Ĵ��� */
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
	  
	  /* ����ʮ��λ */
	  if(val > 0x0000FFFFu){
			  /* ���bits[31:24] */
			  if(val > 0x00FFFFFFu){
					  /* ��ȡbis[31:24]��ֵ����ת����8λ */
					  ix = (CPU_INT08U)(val >> 24u);
					  /* ��ǰ����������ҵ�������ȼ� */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+0u);
				}
				/* ���bits[23:16] */
				else{
				    /* ��ȡbis[23:16]��ֵ����ת����8λ */
					  ix = (CPU_INT08U)(val >> 16u);
					  /* ��ǰ����������ҵ�������ȼ� */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+8u);
				}
		}
		/* ����ʮ��λ */
		else{
			  /* ���bits[15:08] */
			  if(val > 0x000000FFu){
				    /* ��ȡbis[15:08]��ֵ����ת����8λ */
					  ix = (CPU_INT08U)(val >> 8u);
					  /* ��ǰ����������ҵ�������ȼ� */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+16u);
				}
				/* ���bits[07:00] */
				else{
				    /* ��ȡbis[07:00]��ֵ����ת����8λ */
					  ix = (CPU_INT08U)(val >> 0u);
					  /* ��ǰ����������ҵ�������ȼ� */
					  nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix]+24u);
				}
		}
		
		return (nbr_lead_zeros);
}
#endif
