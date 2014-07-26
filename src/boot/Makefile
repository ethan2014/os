# Makefile for the bootlader

AS := as
LD := ld
CC := gcc
FLAGS := -nostdinc -march=i386 -m32 -O0 -n -nostdlib -nostartfiles \
	-nodefaultlibs -Wall -ffreestanding

all: boot.bin

boot.bin: boot.out
	objcopy -O binary -j .text boot.out boot.bin

boot.out: boot.o
	$(LD) -o boot.out boot.o -Ttext 0x7C00

boot.o: boot.s
	$(AS) -o boot.o boot.s
