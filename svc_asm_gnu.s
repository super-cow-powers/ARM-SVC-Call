	/** @file svc_asm_gnu.s
	  * @brief Contains the list of Service Handlers and the dispatch logic.	*/
	//Translated to GNU-style
	.syntax unified
	.section .text.OS_func
	.eabi_attribute Tag_ABI_align_preserved, 1

	//Export SVC Handler
	.global SVC_Handler
	// Import SVC routines
	.global _svc_OS_enable_systick
	.global _svc_OS_addTask
	.global _svc_OS_task_exit
	.global _svc_OS_yield
	.global _svc_OS_schedule
	.global _svc_SCH_wait
	.global _svc_SCH_notify
	.global _svc_SCH_sleep
	.global _svc_usart_acq
	.global _svc_usart_rel
	.global _svc_swrst
	.global _svc_tst_num

	.type  SVC_Handler, %function
SVC_Handler:
	// Link register contains special 'exit handler mode' code
	// Bit 2 tells whether the MSP or PSP was in use
	TST     lr, #4
	ITE EQ
	MRSEQ   r0, MSP
	MRSNE   r0, PSP
	// r0 now contains the SP that was in use
	// Return address is on the stack: load it into r1
	LDR     r1, [r0, #24]
	// Use the return address to find the SVC instruction
	// SVC instruction contains an 8-bit code
	LDRB    r1, [r1, #-2]
	// Check if it's in the table
	CMP     r1, #((SVC_tableEnd - SVC_tableStart)/4)
	IT GE
	// If not, return
	BXGE    lr
	// Branch to the right handler
	// Remember, the SP is in r0
	LDR     r2, =SVC_tableStart
	LDR     pc, [r2, r1, lsl #2]
	
	.balign 8
	.type  SVC_tableStart, %object
SVC_tableStart:	
	.word _svc_OS_enable_systick
	.word _svc_OS_addTask
	.word _svc_OS_task_exit
	.word _svc_OS_yield
	.word _svc_OS_schedule
	.word _svc_SCH_wait
	.word _svc_SCH_notify
	.word _svc_SCH_sleep
	.word _svc_usart_acq
	.word _svc_usart_rel
	.word _svc_swrst
	.word _svc_tst_num
SVC_tableEnd:	

