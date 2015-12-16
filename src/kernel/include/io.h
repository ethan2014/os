#ifndef IO_H
#define IO_H

void outb(int port, unsigned char data);

unsigned char inb(int port);

unsigned char get_bit(const unsigned char byte, const unsigned char pos);
unsigned char set_bit(const unsigned char byte, const unsigned char pos);
unsigned char clear_bit(const unsigned char byte, const unsigned char pos);
unsigned char toggle_bit(const unsigned char byte, const unsigned char pos);

#endif
