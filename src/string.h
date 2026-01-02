#ifndef STRING_H
#define STRING_H

#include "types.h"

/* Fills memory with a constant byte - useful for proc_init */
void* memset(void* dest, int val, size_t len);

/* Copies memory area - crucial for Context Switching & IPC */
void* memcpy(void* dest, const void* src, size_t len);

/* Returns the length of a string - useful for serial logging */
size_t strlen(const char* s);

#endif