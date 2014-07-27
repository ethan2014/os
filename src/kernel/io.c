#ifndef IO_C
#define IO_C

#include <io.h>

void outb(int port, unsigned char data)
{
	asm("outb %%al, %%dx"
	    : // no output data
	    : "d" (port), "a" (data)
	    : // no clobbered
		);
}

char inb(int port, unsigned char data)
{
	char ret;
	
	asm("inb %%dx, %%al"
	    : "=m" (ret)
	    : "d" (port), "a" (data)
	    : // no clobbered
		);

	return ret;
}

#endif
