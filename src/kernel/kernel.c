#ifndef KERNEL_C
#define KERNEL_C

#include <stdlib.h>
#include <kernel.h>
#include <string.h>
#include <vga.h>
#include <keyboard.h>

void kernel_main(void)
{
    vga_init();
    
    kputs("OS by Ethan Lape and Kyle Sandstrom");
    kputs("-----------------------------------");
    
    kprintf("location of kernel_mail: %i\n", (int) kernel_main);
    
    for(;;);
}

void dump_registers(void)
{
    int eax;
    int ebx;
    int ecx;
    int edx;
    int edi;
    int esi;
    int esp;
    int ds;
    int ss;
    int es;
    int fs;
    
    asm volatile (
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        "movl %%edi, %4\n\t"
        "movl %%esi, %5\n\t"
        "movl %%esp, %6\n\t"
        "movw %%ds, %7\n\t"
        "movw %%ss, %8\n\t"
        "movw %%es, %9\n\t"
        "movw %%fs, %10\n\t"
        : "=m" (eax), "=m" (ebx), "=m" (ecx), "=m" (edx), "=m" (edi),
          "=m" (esi), "=m" (esp), "=m" (ds),  "=m" (ss),  "=m" (es), "=m" (fs)
        : /* no inputs */
        : /* no clobbered */
    );
    
    kputs("register dump:");
    kprintf("eax = %i\n", eax);
    kprintf("ebx = %i\n", ebx);
    kprintf("ecx = %i\n", ecx);
    kprintf("edx = %i\n", edx);
    kprintf("edi = %i\n", edi);
    kprintf("esi = %i\n", esi);
    kprintf("esp = %i\n", esp);
    kprintf("ds = %i\n", ds);
    kprintf("ss = %i\n", ss);
    kprintf("es = %i\n", es);
    kprintf("fs = %i\n", fs);
}

#endif

