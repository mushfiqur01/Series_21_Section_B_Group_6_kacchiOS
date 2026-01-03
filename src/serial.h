#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

/* Initializes the COM1 port for QEMU output */
void serial_init(void);

/* Sends a single character to the serial port */
void serial_putc(char c);

/* Sends a null-terminated string to the serial port */
void serial_puts(const char* s);

/* Sends a 32-bit unsigned integer to the serial port as a string */
void serial_print_u32(uint32_t v);

/* Prints a 32-bit integer in hexadecimal format */
void serial_print_hex(uint32_t n);

/* Returns 1 if a character is waiting to be read */
int serial_received(void);

/* Reads a single character from the serial port (blocks until one is available) */
char serial_read(void);

/* Reads a string from the serial port until Enter is pressed */
void serial_gets(char* buf, int n);

#endif
