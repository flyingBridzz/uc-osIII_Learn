;*************************************************
;                 PUBLIC FUNCTIONS
;*************************************************
	EXPORT  CPU_IntDis
	EXPORT  CPU_IntEn
		
	EXPORT  CPU_SR_Save	
	EXPORT  CPU_SR_Restore
		
	EXPORT  CPU_CntLeadZeros
	EXPORT  CPU_CntTrailZeros

;***********************end***********************

	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8
	
CPU_IntDis
	CPSID  I
	BX     LR
	
CPU_IntEn
    CPSIE  I
	BX     LR
	
	

CPU_SR_Save					;关中断
    MRS    R0, PRIMASK
	CPSID  I
	BX     LR

CPU_SR_Restore				;开中断
    MSR    PRIMASK, R0
	BX     LR
	

;****************************************************
;                    计算前导0函数
; 函数声明：CPU_DATA CPU_CntLeadZeros(CPU_DATA val);
;
;****************************************************
CPU_CntLeadZeros
	CLZ  R0, R0         	;计算前导0个数
	BX   LR

;****************************************************
;                    计算后导0函数
; 函数声明：CPU_DATA CPU_CntTrailZeros(CPU_DATA val);
;
;****************************************************
CPU_CntTrailZeros
    RBIT  R0, R0
	CLZ   R0, R0
	BX    LR
	
	END