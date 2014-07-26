#!/bin/bash

sudo mount -t vfat /dev/sdd /media/floppy

sudo cp kernel.bin /media/floppy

sleep 4s

sudo umount /media/floppy
