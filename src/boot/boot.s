.code16
.text
.org 0x0
.global main

.set	fat_segment, 0x0EE0
.set	file_segment,	0x300
.set	root_segment,	0x100
	
main:
	jmp start
	nop

### the BPB for the floppy
oem:		.ascii "Ethan OS" 	# OEM name
sect_size:     	.word  0x200         	# bytes per sector
spc:    	.byte  1             	# sectors per cluster
res_sect:      	.word  1             	# # of reserved sectors
fat_cnt:       	.byte  2             	# # of FAT copies
root_size:     	.word  224           	# size of root directory
total_sect:    	.word  2880          	# total # of sectors if over 32 MB
media:        	.byte  0xF0          	# media Descriptor
fat_size:      	.word  9             	# size of each FAT
spt:		.word  0x12            	# sectors per track
head_cnt:      	.word  2             	# number of read-write heads
hidden_sect:   	.int   0             	# number of hidden sectors
sect32:       	.int   0             	# # sectors for over 32 MB
boot_drive:    	.byte  0             	# holds drive that the boot sector came from
reserved:     	.byte  0             	# reserved, empty
boot_sign:     	.byte  0x29          	# extended boot sector signature
vol_id:        	.ascii "seri"        	# disk serial
volume_label: 	.ascii "MYVOLUME   " 	# volume label
fs_type:      	.ascii "FAT12   "    	# file system type
	
start:
	cli
	movb	boot_drive, %dl
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	movw	$0x7C00, %sp
	sti

	## display loading os message
	# movw	$load_msg, %si
	# call	write_string

	## reset the floppy controller
	movb	boot_drive, %dl
	xorw	%ax, %ax
	int	$0x13
	jc	boot_failed	# jump to failure if carry flag is set

	## at this point, we have confirmed our write_string function works,
	## and the floppy controller is reset, now we need to start looking
	## for the kernel on the floppy disk

	## first, caluculate the size of the root directory in sectors
	movw	$32, %ax
	xorw	%dx, %dx
	mulw	root_size
	divw	sect_size
	movw	%ax, %cx
	movw	%cx, root_sectors # we now know how many sectors the root dir is

	## next, caluculate the starting sector of the root dir
	## root_start = reserved sectors + hidden sectors + (FATs * size of FAT)
	xorw	%ax, %ax
	movb	fat_cnt, %al
	movw	fat_size, %bx
	mulw	%bx
	addw	hidden_sect, %ax
	addw	res_sect, %ax
	movw	%ax, root_start	# we know know what sector the root dir start at

	## now that we know where and how big the root dir is, we can begin reading
	## file entries from it to find the kernel, ax still contains the sector number
	## of the root dir's first sector
	movw	$root_segment, %ax # load the root dir segments here in memory
	movw	%ax, %es
read_next_sector:	
	pushw	%cx
	pushw	%ax
	xorw	%bx, %bx
	movw	root_start, %ax
	call	read_sector	# read sector number ax

check_entry:
	movw	$11, %cx	# only check first 11 byte of file entry
	movw	%bx, %di	# es:di = Directory entry address
	movw	$filename, %si	# ds:si = address of file name
	repz	cmpsb		# compare the two names
	je	found_file	# file entry found, quit looking
	addw	$32, %bx	# go to next entry
	cmpw	sect_size, %bx	# do we need to go to the next sector?
	jne	check_entry	# no. check next file entry

	## yes
	popw	%ax
	incw	%ax		# increment ax to point to next sector
	popw	%cx
	loopnz	read_next_sector
	jmp	boot_failed

	## if the file was found, the file entry stores the first cluster
	## for the file at offset 0x1A (byte 26) in the 32 byte entry
found_file:
	movw	%es:0x1A(%bx), %ax
	movw	%ax, file_start

	## now that we know where our file is located, we need to start using
	## the FAT to get all the pieces of it (it is most likely bigger than just
	## one cluster!)

	## first, tell the BIOS where to copy the FAT to in memory
	movw	$fat_segment, %ax
	movw	%ax, %es	# we will put it at segment 0x0EE0:0x0000
				# remeber, we have 0x7C00 bytes of stack space!

	## calculate the offset of the FAT
	movw	res_sect, %ax
	addw	hidden_sect, %ax

	## read all the sectors of the FAT
	movw	fat_size, %cx
	xorw	%bx, %bx
read_next_fat_sector:
	pushw	%cx
	pushw	%ax
	call	read_sector	# read 1 sector
	popw	%ax
	popw	%cx
	incw	%ax		# increment pointer to next sector
	addw	sect_size, %bx
	loopnz	read_next_fat_sector # keep loading sectors until we have all of them

	## now we just need to read the file into memory
	movw	$file_segment, %ax
	movw	%ax, %es
	xorw	%bx, %bx

	movw	file_start, %cx
read_file_next_sector:
	movw	%cx, %ax
	addw	root_start, %ax
	addw	root_sectors, %ax
	subw	$2, %ax

	pushw	%cx
	call	read_sector
	popw	%cx
	addw	sect_size, %bx

	pushw	%ds
	movw	$fat_segment, %dx
	movw	%dx, %ds

	movw	%cx, %si
	movw	%cx, %dx
	shrw	%dx
	addw	%dx, %si

	movw	%ds:(%si), %dx
	testw	$1, %dx
	jz      read_next_file_even
	andw	$0x0FFF, %dx
	jmp     read_next_file_cluster_done
read_next_file_even:
	shrw	$4, %dx
read_next_file_cluster_done:
	popw	%ds
	movw	%dx, %cx
	cmpw	$0x0FF8, %cx
	jl      read_file_next_sector

	#movw	$booted_msg, %si
	#call	write_string

	## we have loaded the kernel into memory!! now start executing it
	movw	$file_segment, %ax
	movw	%ax, %ds
	movw	%ax, %es
	ljmp	$file_segment, $0

boot_failed:
	#movw	$disk_error, %si
	#call	write_string
	call	reboot

### function to write a string to the screen
#write_string:	
#	lodsb
#	orb	%al, %al
#	jz	write_string_done
#
#	movb	$0xE, %ah
#	movw	$9, %bx
#	int	$0x10
#
#	jmp	write_string
#write_string_done:
#	ret

### function to reboot
reboot:
	#movw	$reboot_msg, %si
	#call	write_string
	xorw	%ax, %ax
	int	$0x16

	.byte	0xEA		# machine language to jump to FFFF:0000 (reboot)
	.word	0x0000
	.word	0xFFFF

### function to read a physical sector off the floppy
### converts the logical sector found in ax to head, sector and track.
### stores the 512 bytes read in es:bx.  will attempt to read 3 times before
### moving to the boot failure function
read_sector:
	xorl	%ecx, %ecx

read:
	pushw	%ax
	pushw	%cx
	pushw	%bx

	## Calculate cylinder, head and sector:
	## Cylinder = (LBA / SectorsPerTrack) / NumHeads
	## Sector   = (LBA mod SectorsPerTrack) + 1
	## Head     = (LBA / SectorsPerTrack) mod NumHeads

	movw	spt, %bx
	xorw	%dx, %dx
	divw	%bx
	
	incw	%dx
	movb	%dl, %cl

	movw	head_cnt, %bx
	xorw	%dx, %dx
	divw	%bx

	movb	%al, %ch
	movb	%dl, %dh

	movw	$0x0201, %ax
	movb	boot_drive, %dl
	popw	%bx
	int	$0x13
	jc	read_fail

	## on success, return to caller
	popw	%cx
	popw	%ax
	ret
	
read_fail:
	popw	%cx
	incw	%cx
	cmpw	$4, %cx
	je	boot_failed

	## reset disk
	xorw	%ax, %ax
	int	$0x13

	popw	%ax
	jmp	read
	
### program data
#load_msg:	.asciz	"loading\r\n"
#disk_error:	.asciz	"disk error.\r\n"
#reboot_msg:	.asciz	"rebooting..\r\n"
#booted_msg:	.asciz	"booted\r\n"
filename:	.asciz	"KERNEL  BIN"
root_sectors:	.word	0
root_start:	.word	0
file_start:	.word	0
	
.fill (510-(.-main)), 1, 0  	# Pad with nulls up to 510 bytes
BootMagic:  	.word 	0xAA55	# magic word for BIOS
	
