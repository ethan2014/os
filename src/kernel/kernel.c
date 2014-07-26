asm(".code32");

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEM 0xB8000

enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

static int text_x;
static int text_y;

void clear_screen();

void println(char *str);

void print_string(char *str, unsigned char color);

void set_video_buffer(int x, int y, char c, char color);

void kernel_main(void)
{
	text_x = 0;
	text_y = 0;
	
	clear_screen();
	println("hello world!!");
	println("new line here");
}

char make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

short make_vga_entry(char c, char color)
{
	short c16 = c;
	short color16 = color;
	return c16 | color16 << 8;
}

void clear_screen()
{
	char color = make_color(COLOR_BLUE, COLOR_BLACK);
	
	for (text_y = 0; text_y < VGA_HEIGHT; text_y++) {
		for (text_x = 0; text_x < VGA_WIDTH; text_x++) {
			set_video_buffer(text_x, text_y, (char)' ', color);
		}
	}

	text_x = 0;
	text_y = 0;
}

void println(char *str)
{
	char color = make_color(COLOR_WHITE, COLOR_BLACK);

	print_string(str, color);
	print_string("\n", color);
}

void print_string(char *str, unsigned char color)
{
	while (*str) {
		char c = *str++;

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
}

void set_video_buffer(int x, int y, char c, char color)
{
	unsigned short* video_mem = (unsigned short*) 0xB8000;

	short value = make_vga_entry(c, color);

	*(video_mem + (y * VGA_WIDTH + x)) = value;
}
