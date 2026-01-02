#include "memory.h"
#include "string.h"
#include "serial.h"

typedef struct mem_block {
    size_t size;
    bool free;
    struct mem_block* next;
} mem_block_t;

static uint8_t heap_pool[HEAP_SIZE];
static mem_block_t* block_head = NULL;

void mem_init(void) {
    block_head = (mem_block_t*)heap_pool;
    block_head->size = HEAP_SIZE - sizeof(mem_block_t);
    block_head->free = true;
    block_head->next = NULL;
    serial_puts("[MEM] Manager initialized\n");
}

void* malloc(size_t size) {
    size = (size + 3) & ~3; // 4-byte Alignment
    mem_block_t* curr = block_head;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(mem_block_t) + 4) {
                mem_block_t* next_block = (mem_block_t*)((uint8_t*)curr + sizeof(mem_block_t) + size);
                next_block->size = curr->size - size - sizeof(mem_block_t);
                next_block->free = true;
                next_block->next = curr->next;
                curr->size = size;
                curr->next = next_block;
                
                serial_puts("[MEM] Block split: allocated "); /* For demo */
                serial_print_u32((uint32_t)size);
                serial_puts(" bytes\n");
            }
            curr->free = false;
            memset((uint8_t*)curr + sizeof(mem_block_t), 0, size);
            return (void*)((uint8_t*)curr + sizeof(mem_block_t));
        }
        curr = curr->next;
    }
    serial_puts("[MEM] ERROR: Allocation failed!\n"); /* Error log */
    return NULL;
}

void free(void* ptr) {
    if (!ptr || (uint8_t*)ptr < heap_pool || (uint8_t*)ptr >= (heap_pool + HEAP_SIZE)) {
        serial_puts("[MEM] ERROR: Invalid free attempt\n");
        return;
    }

    mem_block_t* block = (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));

    if (block->free) return; 

    block->free = true;
    serial_puts("[MEM] Freed block at address\n"); /* For demo */

    mem_block_t* curr = block_head;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += curr->next->size + sizeof(mem_block_t);
            curr->next = curr->next->next;
            serial_puts("[MEM] Coalescing adjacent free blocks\n");
        } else {
            curr = curr->next;
        }
    }
}

void mem_status(void) {
    size_t total_free = 0;
    size_t total_used = 0;
    mem_block_t* curr = block_head;

    serial_puts("\n--- HEAP MEMORY STATUS ---\n");
    while (curr) {
        if (curr->free) total_free += curr->size;
        else total_used += curr->size;
        curr = curr->next;
    }

    serial_puts("Used: ");
    serial_print_u32((uint32_t)total_used);
    serial_puts(" bytes | Free: ");
    serial_print_u32((uint32_t)total_free);
    serial_puts(" bytes\n");
    serial_puts("--------------------------\n");
}