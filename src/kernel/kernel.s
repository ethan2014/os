.code16
.text
.org 0x0

.global _start
_start:
	cli
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	$0x9000, %ax
	movw	%ax, %ss
	movw	$0xFFFF, %sp
	sti
	
	## now that we are in the kernel, we need to move to
	## protected moden

	## load the gdt into memory
	cli
	pusha
	lgdt	gdt_desc
	sti
	popa

	## enter protected mode
	cli
	movl	%cr0, %eax
	orl	$1, %eax
	movl	%eax, %cr0

	ljmp	$0x08, $fix_seg_regs

.code32
fix_seg_regs:
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	movl	$0x90000, %esp

	call	kernel_main

hang:	jmp	hang

gdt:				# address of the gdt
	.word	0
	.word	0
	.word	0
	.word	0

gdt_code:                	# code segment
        .word	0xFFFF		# limit 0-15
        .word	0		# base 0-15
        .byte	0		# base 16-23
        .byte	0b10011010	# access byte
	.byte	0b11001111	# limit 16-19 and flags
	.byte	0		# base 24-31

gdt_data:			# data segment
        .word	0xFFFF		# limit 0-15
        .word	0		# base 0-15
        .byte	0		# base 16-23
        .byte	0b10010010	# access byte
	.byte	0b11001111	# limit 16-19 and flags
	.byte	0		# base 24-31

gdt_end:
gdt_desc:
        .word	 gdt_end - gdt - 1
        .int	 gdt  
