;***********************************************************
;                  ȫ�ֱ���&����
;***********************************************************
    IMPORT    OSTCBCurPtr         ;�ⲿ�ļ�����Ĳο�
    IMPORT    OSTCBHighRdyPtr
		
    IMPORT    OSPrioCur
	IMPORT    OSPrioHighRdy
	
    EXPORT    OSStartHighRdy      ;���ļ�����ĺ���
    EXPORT    PendSV_Handler
;*****************ȫ�ֱ���&����end**************************


;******************��������*********************************
NVIC_INT_CTRL    EQU    0xE000ED04; �жϿ��Ƽ�״̬�Ĵ��� SCB_ICSR
NVIC_SYSPRI14    EQU    0xE000ED22; ϵͳ���ȼ��Ĵ��� SCB_SHPR3:bit16~23,PRI14��PendSV�ж�
NVIC_PENDSV_PRI  EQU    0xFF	  ; PendSV ���ȼ���ֵ����ͣ�
NVIC_PENDSVSET   EQU    0x10000000; ����PendSV�쳣��ֵ Bit28:PENDSVSET
;*****************��������end*******************************

;***********************************************************
;                  �������ָ��
;***********************************************************
    PRESERVE8  ;8�ֽڶ���ָ��ڻ���c���Ի�ϱ���ʱ��Ҫ��
    THUMB;ʹ��thumbָ��
	
	AREA CODE, CODE, READONLY  ;����һ�������
;��ʽ:AREA �������, CODE, READONLY
;***********************************************************
;1������PendSV�쳣�����ȼ�Ϊ���
;2���ٿ�ʼ��һ���������л�֮ǰ������psp=0
;3������PendSV�쳣����ʼ�������л�
;***********************************************************
OSStartHighRdy
  LDR    R0, = NVIC_SYSPRI14
  LDR    R1, = NVIC_PENDSV_PRI
  STRB   R1, [R0]
  
  MOVS   R0, #0
  MSR    PSP, R0
  
  LDR    R0, = NVIC_INT_CTRL
  LDR    R1, = NVIC_PENDSVSET
  STR    R1, [R0]
  
  CPSIE  I
  
OSStartHang
  B      OSStartHang
  
;***********************************************************
;                    PendSVHandler�쳣
;***********************************************************
PendSV_Handler
  CPSID  I ;���жϣ�NMI��HardFault���⣬��ֹ�������л����ж�
  
  MRS    R0, PSP ;��PSP��ֵ���ص�R0
  
  ;�ж�R0�����ֵΪ0����ת��OS_CPU_PendSVHandler_nosave
  ;���е�һ�������л���ʱ��R0�϶�Ϊ0
  CBZ    R0, OS_CPU_PendSVHandler_nosave ;CBZ������Ϊ�����ת
;---------------һ����������--------------------------------
;������л���������һ��Ҫ���е������ջ���ݼ��ص�CPU�Ĵ�����
;-----------------------------------------------------------
;�ڽ���PendSV�쳣��ʱ�򣬵�ǰCPU��xPSR��PC��������ڵ�ַ����
;R14��R12��R3��R2��R1��R0���Զ��洢�ڵ���ǰ�����ջ��
;ͬʱ�ݼ�PSP��ֵ,ͨ�����룺MRS R0, PSP��PSP��ֵ����R0

;�ֶ��洢CPU�Ĵ���R4-R11��ֵ����ǰ�����ջ
  STMDB    R0!, {R4-R11}

; ���� OSTCBCurPtr ָ��ĵ�ַ��R1������LDR����αָ��
  LDR      R1, = OSTCBCurPtr
; ���� OSTCBCurPtr ָ�뵽R1������LDR����ARMָ��
  LDR      R1, [R1]
; �洢R0��ֵ��OSTCBCurPtr->OSTCBStkPtr, ���ʱ��R0������������ջ��ջ��
  STR      R0, [R1]

;-------------�����л�����----------------------------------
;ʵ�� OSTCBCurPtr = OSTCBHighRdyPtr
;����һ��Ҫ���е�����Ķ�ջ���ݼ��ص�CPU�Ĵ�����
;-----------------------------------------------------------
OS_CPU_PendSVHandler_nosave

;OSPrioCur = OSPrioHighRdy
  LDR  R0, = OSPrioCur
  LDR  R1, = OSPrioHighRdy
  LDRB R2, [R1]
  STRB R2, [R0]

;OSTCBCurPtr = OSTCBHighRdyPtr
;���� OSTCBCurPtr ָ��ĵ�ַ��R0������LDR����αָ��
  LDR      R0, = OSTCBCurPtr
;���� OSTCBHighRdyPtr ָ��ĵ�ַ��R1������LDR����αָ��
  LDR      R1, = OSTCBHighRdyPtr
;���� OSTCBHighRdyPtr ָ�뵽R2
  LDR      R2, [R1]
;�洢 OSTCBHighRdyPtr �� OSTCBCurPtr
  STR      R2, [R0]
  
;���� OSTCBStkPtr �� R0
  LDR      R0, [R2]
;������Ҫ�ֶ��������Ϣ��CPU�Ĵ���R4-R11
  LDMIA    R0!, {R4-R11}

;����PSP��ֵ�����ʱ��PSPָ����һ��Ҫָ�������Ķ�ջ��ջ��
;�����ջ���Ѿ����ϸո��ֶ����ص�CPU�Ĵ���R4-R11��ƫ�ƣ�
  MSR      PSP, R0
  
;ȷ���쳣����ʹ�õĶ�ջָ����PSP����LR�Ĵ�����λ2ҪΪ1
  ORR      LR, LR, #0x04 ;ȷ��ѡ����ǽ��̶�ջָ�����������ջָ��
;���ж�
  CPSIE    I
;�쳣���أ����ʱ�������ջ�е�ʣ�����ݽ����Զ����ص�xPSR��
;PC��������ڵ�ַ����R14��R12��R3��R2��R1��R0��������βΣ�
;ͬʱPSP��ֵҲ�����£���ָ�������ջ��ջ����
;��STM32�У���ջ���ɸߵ�ַ��͵�ַ�����ġ�
  BX       LR
  
  NOP   ;Ϊ�˻��ָ����룬�����о���
  
  ENDP   ;����ļ�����
  