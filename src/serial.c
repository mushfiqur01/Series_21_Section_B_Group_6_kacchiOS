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

/* Check if the Line Status Register says data is ready */
int serial_received(void) {
    return inb(COM1 + 5) & 1;
}

/* Wait for data to arrive, then read it from the port */
char serial_read(void) {
    while (serial_received() == 0); // Wait until data is available
    return inb(COM1);               // Read the character from the data register
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

void serial_print_hex(uint32_t n) {
    char* hex_chars = "0123456789ABCDEF";
    serial_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        serial_putc(hex_chars[(n >> i) & 0xF]);
    }
}

void serial_gets(char* buf, int n) {
    int i = 0;
    while (i < n - 1) {
        char c = serial_read();
        
        // Handle Enter key
        if (c == '\r' || c == '\n') {
            break;
        }
        
        if (c == '\b' && i > 0) {
            serial_putc('\b');
            serial_putc(' ');
            serial_putc('\b');
            i--;
            continue;
        }

        buf[i++] = c;
        serial_putc(c); // Echo character back to screen
    }
    buf[i] = '\0'; // Null-terminate the string
    serial_puts("\n");
}
