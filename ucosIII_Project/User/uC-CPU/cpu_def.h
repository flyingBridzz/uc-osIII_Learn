#ifndef _CPU_DEF_H_
#define _CPU_DEF_H_
/*
************************************************************
*                       CPU 字配置
************************************************************
*/ 
#define  CPU_WORD_SIZE_08          (1u)
#define  CPU_WORD_SIZE_16          (2u)
#define  CPU_WORD_SIZE_32          (4u)
#define  CPU_WORD_SIZE_64          (8u)

/* 一个字节长度为8为 */
#define  DEF_OCTET_NBR_BITS        (8u)

/* CPU整型数位定义 */
#define  DEF_INT_CPU_NBR_BITS      (CPU_CFG_DATA_SIZE * DEF_OCTET_NBR_BITS)

#endif
