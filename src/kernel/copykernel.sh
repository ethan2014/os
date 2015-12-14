#!/bin/bash

# copy to physical floppy
#sudo mount -t vfat /dev/sdd /media/floppy
#sudo cp kernel.bin /media/floppy
#sleep 4s
#sudo umount /media/floppy

# copy to mounted floppy image
sudo cp kernel.bin /media/floppy
