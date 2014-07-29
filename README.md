os
==

A very simple, free and open source operating system.

This operating system includs the source code for both the bootloader, which currenly only works
for a 1.44MB floppy disk, and the kernel.

A few features that have been implemented so far:
- a working bootloader
- switching the cpu from real mode to protected mode
- simple string printing functions

This project is still in its VERY early stages, and I am currenly the only one working on it, so progress
will be pretty slow.

This OS is not very complicated yet, all that is needed is a 1.44MB floppy disk and a way to write raw data to it (I use the linux command dd but similar functionality also exists for windows).  putting the compiled bootloader on the first sector of the floppy and copying the compiled kernel (kernel.bin) to the root directory of the floppy is enough to make the OS bootable on any x86 based computer (I use an old Thinkpad 760EL).
