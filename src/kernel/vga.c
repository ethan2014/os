#ifndef VGA_C
#define VGA_C

#include <vga.h>
#include <io.h>

static int text_width;
static int text_height;

static int text_x;
static int text_y;

static unsigned char make_color(enum vga_color fg, enum vga_color bg);

static unsigned short make_vga_entry(char c, char color);

static void set_video_buffer_c(int x, int y, char c, char color);

static void set_video_buffer(int x, int y, short data);

static short get_video_buffer(int x, int y);

static void set_cursor(int x, int y);

static void update_cursor(void);

static void scroll_text(void);

static unsigned char make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static unsigned short make_vga_entry(char c, char color)
{
	return c | color << 8;
}

static void set_video_buffer_c(int x, int y, char c, char color)
{
	short value = make_vga_entry(c, color);

	set_video_buffer(x, y, value);
}

static void set_video_buffer(int x, int y, short data)
{
	unsigned short* video_mem = (unsigned short*) 0xB8000;

	*(video_mem + (y * text_width + x)) = data;
}

static short get_video_buffer(int x, int y)
{
	unsigned short* video_mem = (unsigned short*) 0xB8000;

	return *(video_mem + (y * text_width + x));
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

static void scroll_text(void)
{
	vga_clear_screen();
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

	int x, y;
	for (y = 0; y < text_height; y++) {
		for (x = 0; x < text_width; x++) {
			set_video_buffer_c(x, y, (char) ' ', color);
		}
	}

	text_x = 0;
	text_y = 0;

	set_cursor(text_x, text_y);
}

void vga_print_char(const char c)
{
	char color = make_color(COLOR_WHITE, COLOR_BLACK);

	vga_print_char_c(c, color);
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
		set_video_buffer_c(text_x, text_y, c, color);
		text_x++;

		if (text_x >= text_width) {
			text_y++;
			text_x = 0;
		}
	}

	if (text_y >= text_height) {
		scroll_text();
	}

	update_cursor();
}

void vga_print_string(const char *str)
{
	char color = make_color(COLOR_WHITE, COLOR_BLACK);

	vga_print_string_c(str, color);
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
