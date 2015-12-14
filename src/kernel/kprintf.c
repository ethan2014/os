#ifndef KPRINTF_C
#define KPRINTF_C

#include <stddef.h>
#include <string.h>
#include <kprintf.h>
#include <vga.h>

static void parse_arg(const char arg, va_list *list);

static void parse_arg(const char arg, va_list *list)
{
    if (list == NULL) {
	return;
    }
	
    char buffer[16];

    switch (arg) {
    case '%':
	vga_print_char(arg);
	break;
    case 'i':
	itoa(va_arg(*list, int), buffer);
	vga_print_string(buffer);
	break;
    case 'c':
	vga_print_char(va_arg(*list, int));
	break;
    case 's':
	vga_print_string((char*) va_arg(*list, int));
	break;
    }
}

void kprintf(char *format, ...)
{
    if (format == NULL) {
	return;
    }
	
    va_list list;
    va_start(list, format);

    char *str = format;

    char c;
    while ((c = *str++)) {
	if (c == '%') {
	    parse_arg(*str++, &list);
	} else {
	    vga_print_char(c);
	}
    }

    va_end(list);
}

#endif
