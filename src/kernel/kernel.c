#include <string.h>
#include <vga.h>
#include <kprintf.h>

void kernel_main(void)
{
	vga_init();
	kprintf("hello world!\n");
	kprintf("this is a char: %c\n", '^');
	kprintf("this is a string: %s\n", "hello world");
	kprintf("this is a number: %i\n", 1234);
	kprintf("this is a negative number: %i\n", -3874);
}

