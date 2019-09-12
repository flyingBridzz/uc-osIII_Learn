;***********************************************************
;                  全局变量&函数
;***********************************************************
    IMPORT    OSTCBCurPtr         ;外部文件引入的参考
    IMPORT    OSTCBHighRdyPtr
		
    IMPORT    OSPrioCur
	IMPORT    OSPrioHighRdy
	
    EXPORT    OSStartHighRdy      ;该文件定义的函数
    EXPORT    PendSV_Handler
;*****************全局变量&函数end**************************


;******************常量定义*********************************
NVIC_INT_CTRL    EQU    0xE000ED04; 中断控制及状态寄存器 SCB_ICSR
NVIC_SYSPRI14    EQU    0xE000ED22; 系统优先级寄存器 SCB_SHPR3:bit16~23,PRI14是PendSV中断
NVIC_PENDSV_PRI  EQU    0xFF	  ; PendSV 优先级的值（最低）
NVIC_PENDSVSET   EQU    0x10000000; 触发PendSV异常的值 Bit28:PENDSVSET
;*****************常量定义end*******************************

;***********************************************************
;                  代码产生指令
;***********************************************************
    PRESERVE8  ;8字节对齐指令（在汇编和c语言混合编译时需要）
    THUMB;使用thumb指令
	
	AREA CODE, CODE, READONLY  ;定义一个代码段
;格式:AREA 代码段名, CODE, READONLY
;***********************************************************
;1、配置PendSV异常的优先级为最低
;2、再开始第一次上下文切换之前，设置psp=0
;3、触发PendSV异常，开始上下文切换
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
;                    PendSVHandler异常
;***********************************************************
PendSV_Handler
  CPSID  I ;关中断，NMI和HardFault除外，防止上下文切换被中断
  
  MRS    R0, PSP ;将PSP的值加载到R0
  
  ;判断R0，如果值为0则跳转到OS_CPU_PendSVHandler_nosave
  ;进行第一次任务切换的时候，R0肯定为0
  CBZ    R0, OS_CPU_PendSVHandler_nosave ;CBZ如果结果为零就跳转
;---------------一、保存上文--------------------------------
;任务的切换，即把下一个要运行的任务堆栈内容加载到CPU寄存器中
;-----------------------------------------------------------
;在进入PendSV异常的时候，当前CPU的xPSR，PC（任务入口地址），
;R14，R12，R3，R2，R1，R0会自动存储在到当前任务堆栈，
;同时递减PSP的值,通过代码：MRS R0, PSP吧PSP的值传给R0

;手动存储CPU寄存器R4-R11的值到当前任务堆栈
  STMDB    R0!, {R4-R11}

; 加载 OSTCBCurPtr 指针的地址到R1，这里LDR属于伪指令
  LDR      R1, = OSTCBCurPtr
; 加载 OSTCBCurPtr 指针到R1，这里LDR属于ARM指令
  LDR      R1, [R1]
; 存储R0的值到OSTCBCurPtr->OSTCBStkPtr, 这个时候R0存的是任务空闲栈的栈顶
  STR      R0, [R1]

;-------------二、切换下文----------------------------------
;实现 OSTCBCurPtr = OSTCBHighRdyPtr
;把下一个要运行的任务的堆栈内容加载到CPU寄存器中
;-----------------------------------------------------------
OS_CPU_PendSVHandler_nosave

;OSPrioCur = OSPrioHighRdy
  LDR  R0, = OSPrioCur
  LDR  R1, = OSPrioHighRdy
  LDRB R2, [R1]
  STRB R2, [R0]

;OSTCBCurPtr = OSTCBHighRdyPtr
;加载 OSTCBCurPtr 指针的地址到R0，这里LDR属于伪指令
  LDR      R0, = OSTCBCurPtr
;加载 OSTCBHighRdyPtr 指针的地址到R1，这里LDR属于伪指令
  LDR      R1, = OSTCBHighRdyPtr
;加载 OSTCBHighRdyPtr 指针到R2
  LDR      R2, [R1]
;存储 OSTCBHighRdyPtr 到 OSTCBCurPtr
  STR      R2, [R0]
  
;加载 OSTCBStkPtr 到 R0
  LDR      R0, [R2]
;加载需要手动保存的信息到CPU寄存器R4-R11
  LDMIA    R0!, {R4-R11}

;更新PSP的值，这个时候PSP指向下一个要指向的任务的堆栈的栈底
;（这个栈底已经加上刚刚手动加载到CPU寄存器R4-R11的偏移）
  MSR      PSP, R0
  
;确保异常返回使用的堆栈指针是PSP，即LR寄存器的位2要为1
  ORR      LR, LR, #0x04 ;确保选择的是进程堆栈指针而不是主堆栈指针
;开中断
  CPSIE    I
;异常返回，这个时候任务堆栈中的剩下内容将会自动加载到xPSR，
;PC（任务入口地址），R14，R12，R3，R2，R1，R0（任务的形参）
;同时PSP的值也将更新，即指向任务堆栈的栈顶。
;在STM32中，堆栈是由高地址向低地址生长的。
  BX       LR
  
  NOP   ;为了汇编指令对齐，不会有警告
  
  ENDP   ;汇编文件结束
  