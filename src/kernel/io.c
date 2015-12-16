#ifndef IO_C
#define IO_C

#include <io.h>

void outb(int port, unsigned char data)
{
    asm(
        "outb %%al, %%dx"
        : // no output data
        : "d" (port), "a" (data)
        : // no clobbered
    );
}

unsigned char inb(int port)
{
    unsigned char ret;
    
    asm(
        "inb %%dx, %%al\n\t"
        "movb %%al, %0"
        : "=m" (ret)
        : "d" (port)
        : // no clobbered
    );
    
    return ret;
}

unsigned char get_bit(const unsigned char byte, const unsigned char pos)
{
    return (byte >> pos) & 1;
}

unsigned char set_bit(const unsigned char byte, const unsigned char pos)
{
    return (byte | (1 << pos));
}

unsigned char clear_bit(const unsigned char byte, const unsigned char pos)
{
    return (byte & ~(1 << pos));
}

unsigned char toggle_bit(const unsigned char byte, const unsigned char pos)
{
    return (byte ^ (1 << pos));
}

#endif
