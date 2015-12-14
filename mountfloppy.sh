#!/bin/bash

sudo mount -t vfat floppy.img /media/floppy
sudo cp src/kernel/kernel.bin /media/floppy
sudo umount /media/floppy
