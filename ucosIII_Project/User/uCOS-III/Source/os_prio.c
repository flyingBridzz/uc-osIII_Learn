#include "os.h"

/* 定义优先级表 */
CPU_DATA  OSPrioTbl[OS_PRIO_TBL_SIZE];

/*优先级初始化*/
void OS_PrioInit(void)
{
	CPU_DATA i;
	
	/* 默认全部初始化为0 */
	for(i=0u;i<OS_PRIO_TBL_SIZE;i++)
	{
		  OSPrioTbl[i] = (CPU_DATA)0;
	}
}

/* 置位优先级表中相应的位 */
void OS_PrioInsert(OS_PRIO prio)
{
	  CPU_DATA bit;
	  CPU_DATA bit_nbr;
	  OS_PRIO ix;
	  /* 求模操作，获取优先级表数组的下标索引 */
	  ix = prio / DEF_INT_CPU_NBR_BITS;
		
	  /* 求余操作，将优先级限制在DEF_INT_CPU_NBR_BITS之内 */
	  bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
		
	  /* 获取优先级在优先级表中对应的位的位置 */
	  bit = 1u;
	  bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	
	  /* 将优先级在优先级表中对应的位置1 */
	  OSPrioTbl[ix] |= bit;
}

void OS_PrioRemove(OS_PRIO prio)
{
	  CPU_DATA bit;
	  CPU_DATA bit_nbr;
	  OS_PRIO  ix;
	  /* 求模操作，获取优先级表数组的下标索引 */
	  ix = prio / DEF_INT_CPU_NBR_BITS;
	
	   /* 求余操作，将优先级限制在DEF_INT_CPU_NBR_BITS之内 */
	  bit_nbr = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
	
	  /* 获取优先级在优先级表中对应的位的位置 */
	  bit = 1u;
	  bit <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
	  
		/* 将优先级在优先级表中对应的位清0 */
	  OSPrioTbl[ix] &= ~bit;
}

OS_PRIO OS_PrioGetHighest(void)
{
	  CPU_DATA *p_tbl;
	  OS_PRIO   prio;
	
	  prio = (OS_PRIO)0;
	  
	  /* 获取优先级表首地址 */
	  p_tbl = &OSPrioTbl[0];
	
	  /* 找到数值不为0的数组成员 */
	  while(*p_tbl == (CPU_DATA)0){
				prio += DEF_INT_CPU_NBR_BITS;
			  p_tbl++;
		}
		
		/* 找到优先级表中位置的最高的优先级 */
		prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl);
		return (prio);
}

