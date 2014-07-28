#ifndef KEYBOARD_C
#define KEYBOARD_C

#include <keyboard.h>
#include <kprintf.h>
#include <io.h>

static const unsigned char data_port = 0x60;
static const unsigned char status_port = 0x64;

void keyboard_init(void)
{
	// enable first ps/2 port
	outb(status_port, 0xAE);
}

char have_key_stroke(void)
{
	return inb(status_port, 0) & 1;
}

char get_key_stroke(void)
{
	while (1) {
		if (have_key_stroke) {
			return inb(data_port, 0);
		}
	}
}

#endif
