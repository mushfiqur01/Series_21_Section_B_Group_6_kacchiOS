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

#endif