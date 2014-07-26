# Makefile for the bootlader

AS := as
LD := ld
CC := gcc
FLAGS := -nostdinc -march=i386 -m32 -O0 -n -nostdlib -nostartfiles \
	-nodefaultlibs -Wall -ffreestanding

all: kernel.bin

kernel.bin: kernel.o
	objcopy -O binary -j.text -j.data -j.rodata kernel.o kernel.bin

kernel.o: kernel.s kernel.c
	$(CC) $(FLAGS) -e_start -Ttext=0x3000 -o kernel.o kernel.s kernel.c

