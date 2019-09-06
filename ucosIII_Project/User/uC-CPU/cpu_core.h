#ifndef _CPU_CORE_H_
#define _CPU_CORE_H_

#include "lib_def.h"
#include "cpu_cfg.h"
#include "cpu.h"

#ifdef  CPU_CORE_MODULE /* CPU_CORE_MODULE 只在cpu_core.c 文件的开头定义 */
#define CPU_CORE_EXT
#else
#define CPU_CORE_EXT  extern
#endif

#if   ((CPU_CFG_TS_32_EN == DEF_ENABLED) || \
			 (CPU_CFG_TS_64_EN == DEF_ENABLED))
#define  CPU_CFG_TS_EN      DEF_ENABLED
#else
#define  CPU_CFG_TS_EN      DEF_DISABLED
#endif

#if   ((CPU_CFG_TS_EN == DEF_ENABLED) || \
			(defined(CPU_CFG_INT_DIS_MEAS_EN)))
#define  CPU_CFG_TS_TMR_EN  DEF_ENABLED
#else
#define  CPU_CFG_TS_TMR_EN  DEF_DISABLED
#endif

/*
**************************************************
*                   时间戳数据类型
**************************************************
*/

typedef  CPU_INT32U  CPU_TS32;

typedef  CPU_INT32U  CPU_TS_TMR_FREQ;
typedef  CPU_TS32    CPU_TS;
typedef  CPU_INT32U  CPU_TS_TMR;


/*
**************************************************
*                      全局变量
**************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_CORE_EXT  CPU_TS_TMR_FREQ  CPU_TS_TmrFreq_Hz;
#endif

/*
**************************************************
*                      函数声明
**************************************************
*/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR CPU_TS_TmrRd(void);
#endif

void CPU_Init(void);

#if ((CPU_CFG_TS_EN == DEF_ENABLED) || \
     (CPU_CF_TS_TMR_EN == DEF_ENABLED))
static void  CPU_TS_Init(void);
#endif

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrInit(void);
#endif

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void CPU_TS_TmrFreqSet(CPU_TS_TMR_FREQ freq_hz);
#endif

CPU_INT32U  BSP_CPU_ClkFreq(void);

#endif /* _CPU_CORE_H_ */
