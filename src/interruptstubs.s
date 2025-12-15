.set IRQ_BASE, 0x20

.section .text

.extern handle_interrupt

.macro HandleException num
.global handle_exception\num\()
handle_exception\num\():
	movb $\num, (interruptnumber)
	jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global handle_interrupt_request\num\()
handle_interrupt_request\num\():
	movb $\num + IRQ_BASE, (interruptnumber)
	jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
	pusha # Push all the registers ?
	pushl %ds # Push data segments
	pushl %es
	pushl %fs
	pushl %gs

	pushl %esp
	push (interruptnumber)
	call handle_interrupt
	# addl $5, %esp
	movl %eax, %esp

	popl %gs
	popl %fs
	popl %es
	popl %ds 
	popa 

.global ignore_interrupt_request
ignore_interrupt_request:
	iret

.data
	interruptnumber: .byte 0
