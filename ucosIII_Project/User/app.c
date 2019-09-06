#include "os.h"
#include "ARMCM4.h"

#define    TASK1_STK_SIZE    128
#define    TASK2_STK_SIZE    128

static     CPU_STK    Task1Stk[TASK1_STK_SIZE];
static     CPU_STK    Task2Stk[TASK2_STK_SIZE];

static     OS_TCB     Task1TCB;
static		 OS_TCB     Task2TCB;

/*
**flag���ڹ۲첨��ʹ��
*/
unsigned int flag1;
unsigned int flag2;

uint32_t TimeStart;						/* ��������ȫ�ֱ��� */
uint32_t TimeEnd;
uint32_t TimeUse;


void delay(unsigned int tim)
{
	 for(; tim!=0; tim--);
}

/* ����1 */
void Task1(void *p_arg)
{
    while(1){
		    flag1 = 1;
			
				//TimeStart = OS_TS_GET();
				OSTimeDly(5);
				//TimeEnd = OS_TS_GET();
				//TimeUse = TimeEnd - TimeStart;
				
				flag1 = 0;
				OSTimeDly(5);
			
			  //OSSched();
		}
}

/* ����2 */
void Task2(void *p_arg)
{
	  while(1){
		    flag2 = 1;
			  OSTimeDly(5);
			  flag2 = 0;
			  OSTimeDly(5);
			
			  //OSSched();
		}
}


int main()
{
    OS_ERR err;
		
		/* ��ʼ��ʱ��� */
		CPU_Init();
	
		/* �ر��ж� */
		CPU_IntDis();
	  
	  /* ����SysTick 10ms �ж�һ�� */
	  OS_CPU_SysTickInit(10);
		
		/* ��ʼ����ص�ȫ�ֱ��� */
		OSInit(&err);
		
		/* �������� */
		OSTaskCreate((OS_TCB *)      &Task1TCB, \
								 (OS_TASK_PTR)   Task1, \
								 (void *)        0, \
								 (CPU_STK *)     &Task1Stk[0], \
								 (CPU_STK_SIZE)  TASK1_STK_SIZE, \
 								 (OS_ERR *)      &err);

		OSTaskCreate((OS_TCB *)      &Task2TCB, \
								 (OS_TASK_PTR)   Task2, \
								 (void *)        0, \
								 (CPU_STK *)     &Task2Stk[0], \
								 (CPU_STK_SIZE)  TASK2_STK_SIZE, \
								 (OS_ERR *)      &err);
		/* ��������뵽�����б� */
		OSRdyList[0].HeadPtr = &Task1TCB;
		OSRdyList[1].HeadPtr = &Task2TCB;
								 
		OSStart(&err);
}

