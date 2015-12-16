.code16
.text
.org 0x0
	.set	MISC_WRITE, 0x3C2
	.set 	SEQ_INDEX, 0x3C4
	.set	SEQ_DATA, 0x3C5
	
	.set	CRTC_INDEX, 0x3D4
	.set	CRTC_DATA, 0x3D5
	.set	INSTAT_READ, 0x3DA

	.set 	NUM_SEQ_REGS, 5
	.set	NUM_CRTC_REGS, 25
	
### _start is the entry point for the os, this is called indirectly
### by the boost loader
.global _start
_start:
	## setup the stack to real mode until we switch to protected mode
	cli
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	$0x9000, %ax
	movw	%ax, %ss
	movw	$0xFFFF, %sp
	sti

	## while we are still in real mode, setup vga
	## first, we will use 80x25 with 8x16 font
	movw 	$0x0003, %ax
	int	$0x10

	## next, 80x50 with 8x8 font
	movw 	$0x1112, %ax
	xorb	%bl, %bl
	int	$0x10

	jmp	done

	## finally, 90x60 text mode
	movw	regs, %si
	call	write_vga_registers

	push 	%ds
	push 	%es

	## tell the bios about the changes to vga
	movw	$0x0040, %ax
	movw	%ax, %ds
	movw	%ax, %es

	## columns on the screen
	movw	$90, 0x004A

	## framebuffer size (90*60*2)
	movw	$10800, 0x004C

	## cursor position for 8 pages
	movw	$8, %cx
	movw	$0x0050, %di
	xorw	%ax, %ax
	rep	stosw

	## cursor shape
	movw	$0x0607, 0x0060

	## rows on screen, minus one
	movb	$59, 0x0084

	## character height, in scan lines
	movb	$8, 0x0085

	pop 	%es
	pop 	%ds

done:	
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

write_vga_registers:
	push	%si
	push 	%dx
	push 	%cx
	push 	%ax
	cld

	## write MISC register
	movw	$MISC_WRITE, %dx
	lodsb
	out	%al, %dx

	## write sequencer registers
	movw	$NUM_SEQ_REGS, %cx
	movb	$0, %ah

write_seq:
	movw	$SEQ_INDEX, %dx
	movb	%ah, %al
	out	%al, %dx

	movw	$SEQ_DATA, %dx
	lodsb
	out	%al, %dx

	incb	%ah
	loop	write_seq

	## write CRTC registers
	## unlock CRTC registers: enable writes to CRTC regs 0-7
	movw	$CRTC_INDEX, %dx
	movb	$17, %al
	out	%al, %dx

	movw	$CRTC_DATA, %dx
	in	%dx, %al
	andb	$0x7F, %al
	out	%al, %dx

	## unlock CRTC registers: enable access to vertical retrace regs
	movw	$CRTC_INDEX, %dx
	movb	$3, %al
	out	%al, %dx

	movw	$CRTC_DATA, %dx
	in	%dx, %al
	orb	$0x80, %al
	out	%al, %dx

	## make sure CRTC registers remain unlocked
	movb	17(%si), %al
	andb	$0x7F, %al
	movb	%al, 17(%si)

	movb	3(%si), %al
	orb	$0x80, %al
	movb	%al, 3(%si)

	## now write the CRTC registers
	movw	$NUM_CRTC_REGS, %cx
	movb	$0, %ah
	
write_crtc:
	movw	$CRTC_INDEX, %dx
	movb	%ah, %al
	out	%al, %dx

	movw	$CRTC_DATA, %dx
	lodsb
	out	%al, %dx

	incb	%ah
	loop	write_crtc

	pop	%ax
	pop	%cx
	pop	%dx
	pop 	%si
	ret

.code32
fix_seg_regs:
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	movl	$0x90000, %esp

	## we are now in protected mode, the GDT is set up and the stack is ready,
	## so now we call the main function of the kernel
	call	kernel_main

hang:
	jmp	hang

regs:
	## MISC
	.byte 	0x0E7

	## SEQuencer
	.byte 	0x03, 0x01, 0x03, 0x00, 0x02

	## CRTC
	.byte 	0x6B, 0x59,  0x5A, 0x82, 0x60,  0x8D, 0x0B,  0x3E
	.byte 	0x00, 0x47,  0x06, 0x07, 0x00,  0x00, 0x00,  0x00
	.byte 	0x0EA, 0x0C, 0x0DF, 0x2D, 0x08, 0x0E8, 0x05, 0x0A3
	.byte 	0x0FF
	
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
