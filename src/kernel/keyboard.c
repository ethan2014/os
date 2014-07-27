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

	char status = inb(status_port, 0);

	kprintf("keyboard status: %i\n", status);

	while (1) {
		status = inb(status_port, 0);

		if (status & 1) {
			char c = inb(data_port, 0);

			kprintf("%i", c);

			if (c == 28) {
				break;
			}
		}
	}
}

#endif
