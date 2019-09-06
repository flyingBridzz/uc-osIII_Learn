#ifndef _CPU_DEF_H_
#define _CPU_DEF_H_
/*
************************************************************
*                       CPU ������
************************************************************
*/ 
#define  CPU_WORD_SIZE_08          (1u)
#define  CPU_WORD_SIZE_16          (2u)
#define  CPU_WORD_SIZE_32          (4u)
#define  CPU_WORD_SIZE_64          (8u)

/* һ���ֽڳ���Ϊ8Ϊ */
#define  DEF_OCTET_NBR_BITS        (8u)

/* CPU������λ���� */
#define  DEF_INT_CPU_NBR_BITS      (CPU_CFG_DATA_SIZE * DEF_OCTET_NBR_BITS)

#endif
