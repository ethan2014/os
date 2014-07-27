#ifndef VGA_C
#define VGA_C

#include <vga.h>
#include <io.h>

static int text_width;
static int text_height;

static int text_x;
static int text_y;

static unsigned char make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static unsigned short make_vga_entry(char c, char color)
{
	return c | color << 8;
}

static void set_video_buffer(int x, int y, char c, char color)
{
	unsigned short* video_mem = (unsigned short*) 0xB8000;

	short value = make_vga_entry(c, color);

	*(video_mem + (y * text_width + x)) = value;
}

static void set_cursor(int x, int y)
{
	unsigned short position = (y * text_width) + x;

	outb(0x3D4, 0x0F); 
	outb(0x3D5, (unsigned char) (position & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char ) ((position >> 8) & 0xFF));
}

static void update_cursor(void)
{
	set_cursor(text_x, text_y);
}

void vga_init(void)
{
	text_width = *((short *) 0x044A);
	text_height = 25;
	text_x = 0;
	text_y = 0;
	vga_clear_screen();
}

void vga_clear_screen(void)
{
	char color = make_color(COLOR_BLACK, COLOR_BLACK);
	
	for (text_y = 0; text_y < text_height; text_y++) {
		for (text_x = 0; text_x < text_width; text_x++) {
			set_video_buffer(text_x, text_y, (char) ' ', color);
		}
	}

	text_x = 0;
	text_y = 0;

	set_cursor(text_x, text_y);
}

void vga_print_char(const char c)
{
	char color = make_color(COLOR_WHITE, COLOR_BLACK);
	
	switch (c) {
	case '\n':
	case '\r':
		text_y++;
		text_x = 0;
		break;
	default:
		set_video_buffer(text_x, text_y, c, color);
		text_x++;
	}
}

void vga_print_char_c(const char c, const unsigned char color)
{
	switch (c) {
	case '\n':
	case '\r':
		text_y++;
		text_x = 0;
		break;
	default:
		set_video_buffer(text_x, text_y, c, color);
		text_x++;
	}
}

void vga_print_string(const char *str)
{
	char color = make_color(COLOR_WHITE, COLOR_BLACK);

	char c;
	char *ptr = str;
	while ((c = *ptr++)) {
		vga_print_char_c(c, color);
	}
}


void vga_print_string_c(const char *str, const unsigned char color)
{
	char c;
	char *ptr = str;
	while ((c = *ptr++)) {
		vga_print_char_c(c, color);
	}
}

void vga_println(const char *str)
{
	vga_print_string(str);
	vga_print_char('\n');

	update_cursor();
}

#endif
