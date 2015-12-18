#ifndef VGA_C
#define VGA_C

#include <stddef.h>
#include <vga.h>
#include <io.h>

static int text_width;
static int text_height;

static int text_x;
static int text_y;

static unsigned char make_color(enum vga_color fg, enum vga_color bg);

static unsigned short make_vga_entry(const char c, const char color);

static void set_video_buffer_color(const int x, const int y, const char c, const char color);

static void set_video_buffer(const int x, const int y, const short int data);

static unsigned short get_video_buffer(const int x, const int y);

static void set_cursor(int x, int y);

static void update_cursor(void);

static void scroll_text(void);

static void scroll_n_text(int lines);

static unsigned char make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static unsigned short make_vga_entry(const char c, const char color)
{
    return c | color << 8;
}

static void set_video_buffer_color(const int x, const int y, const char c, const char color)
{
    set_video_buffer(x, y, make_vga_entry(c, color));
}

static void set_video_buffer(const int x, const int y, const short data)
{
    const int video_mem = 0xB8000;

    *((short int*)video_mem + (y * text_width + x)) = data;
}

static unsigned short get_video_buffer(const int x, const int y)
{
    const int video_mem = 0xB8000;

    return *((short int*)video_mem + (y * text_width + x));
}

static void set_cursor(int x, int y)
{
    if (x >= text_width) {
        x = x - text_width;
        y++;
    }
    
    if (y >= text_height) {
        y = 0;
        scroll_text();
    }
    
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
    scroll_n_text(1);
}

static void scroll_n_text(int lines)
{
    if (lines <= 0) {
        return;
    }
    
    if (lines >= text_height) {
        vga_clear_screen();
        
        return;
    }
    
    static const char b = ' ';
    const char black = make_color(COLOR_BLACK, COLOR_BLACK);
    const unsigned short int blank = make_vga_entry(b, black);
    
    unsigned short int line[80];
    
    int y = 0;
    
    while (lines < text_height) {
        int x;
        for (x = 0; x < text_width; x++) {
            line[x] = get_video_buffer(x, lines);
        }
        
        for (x = 0; x < text_width; x++) {
            set_video_buffer(x, y, line[x]);
        }
        
        y++;
        lines++;
    }
    
    
    text_x = 0;
    text_y = y;
    
    while (y < text_height) {
        int x;
        
        for (x = 0; x < text_width; x++) {
            set_video_buffer(x, y, blank);
        }
        
        y++;
    }

}

void vga_init(void)
{
    //text_width = *((short *) 0x044A);
    text_width = 80;
    text_height = 50;
    text_x = 0;
    text_y = 0;
    //vga_clear_screen();
}

void vga_clear_screen(void)
{
    const char color = make_color(COLOR_BLACK, COLOR_BLACK);
    static const char blank = ' ';

    int x;
    int y;
    
    for (y = 0; y < text_height; y++) {
	for (x = 0; x < text_width; x++) {
            set_video_buffer_color(x, y, blank, color);
	}
    }

    text_x = 0;
    text_y = 0;

    set_cursor(text_x, text_y);
}

void vga_print_char(const char c)
{
    const char color = make_color(COLOR_WHITE, COLOR_BLACK);

    vga_print_char_color(c, color);
}

void vga_print_char_color(const char c, const unsigned char color)
{
    switch (c) {
    case '\n':
    case '\r':
	text_y++;
	text_x = 0;
	break;
    default:
        set_video_buffer_color(text_x, text_y, c, color);
	text_x++;

	if (text_x >= text_width) {
	    text_y++;
	    text_x = 0;
	}
    }

    if (text_y >= text_height) {
        //text_y = 0;
	scroll_text();
    }

    update_cursor();
}

void vga_print_string(char *str)
{
    if (str == NULL) {
	return;
    }
    
    const char color = make_color(COLOR_WHITE, COLOR_BLACK);
    
    vga_print_string_color(str, color);
}


void vga_print_string_color(char *str, const unsigned char color)
{
    if (str == NULL) {
	return;
    }
    
    char *ptr = str;

    char c;
    while ((c = *ptr++)) {
        vga_print_char_color(c, color);
    }
}

void vga_println(char *str)
{
    if (str == NULL) {
	return;
    }
    
    vga_print_string(str);
    vga_print_char('\n');

    update_cursor();
}

#endif
