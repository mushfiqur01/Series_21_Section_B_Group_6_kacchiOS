#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

/* 2MB Heap Size */
#define HEAP_SIZE (2 * 1024 * 1024)

/* Manager Lifecycle */
void mem_init(void);

/* Allocation and Deallocation */
void* malloc(size_t size);
void free(void* ptr);

/* Monitoring Utility */
void mem_status(void);

#endif