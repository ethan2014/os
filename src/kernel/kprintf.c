#ifndef KPRTINF_H
#define KPRINTF_H

#include <vga.h>

void kprintf(const char *format, ...)
{
	char *str = format;
	
	va_list list;
	va_start(list, str);

	while ((char c = *str++)) {
		if (c == '%') {

		} else {
			
		}
	}

	va_end(list);
}

#endif
