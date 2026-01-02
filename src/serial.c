#include "serial.h"

#define COM1 0x3f8

/* Inline assembly to write to an I/O port */
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* Inline assembly to read from an I/O port */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(COM1 + 1, 0x00);    /* Disable all interrupts */
    outb(COM1 + 3, 0x80);    /* Enable DLAB (set baud rate divisor) */
    outb(COM1 + 0, 0x03);    /* Set divisor to 3 (38400 baud) lo byte */
    outb(COM1 + 1, 0x00);    /* hi byte */
    outb(COM1 + 3, 0x03);    /* 8 bits, no parity, one stop bit */
    outb(COM1 + 2, 0xC7);    /* Enable FIFO, clear them, with 14-byte threshold */
}

static int is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

void serial_putc(char c) {
    while (is_transmit_empty() == 0);
    outb(COM1, c);
}

void serial_puts(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        serial_putc(s[i]);
    }
}

void serial_print_u32(uint32_t v) {
    char buf[12];
    int pos = 0;
    if (v == 0) {
        serial_putc('0');
        return;
    }
    while (v) {
        buf[pos++] = '0' + (v % 10);
        v /= 10;
    }
    while (pos--) {
        serial_putc(buf[pos]);
    }
}