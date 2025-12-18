.set IRQ_BASE, 0x20

.section .text

.extern handle_interrupt

.macro HandleException num
.global handle_exception\num\()
handle_exception\num\():
	movb $\num, (interrupt_number)
	jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global handle_interrupt_request\num\()
handle_interrupt_request\num\():
	movb $\num + IRQ_BASE, (interrupt_number)
	jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
	pushfl
	pushal

	push %ds
	push %es
	push %fs
	push %gs
	
	pushl %esp
	push (interrupt_number)
	call handle_interrupt
	movl %eax, %esp

	pop %gs
	pop %fs
	pop %es
	pop %ds

	popal
	popfl

.global ignore_interrupt_request
ignore_interrupt_request:
	iret

.data
	interrupt_number: .byte 0
