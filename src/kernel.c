#include "serial.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"

/* Signal handler for IPC Demonstration */
void demo_signal_handler(int sig, uint32_t data) {
    serial_puts("\n[DEMO] IPC Signal Received! Sig: ");
    serial_print_u32((uint32_t)sig);
    serial_puts(" Data: ");
    serial_print_u32(data);
    serial_puts("\n");
}

/* Simulated Workload Task */
void task_entry() {
    while(1) {
        serial_puts(".");
        for(volatile int i = 0; i < 2000000; i++); // CPU Delay
    }
}

void kernel_main() {
    serial_init();
    serial_puts("=== kacchiOS Final Integration Demo ===\n");

    // --- FEATURE: MEMORY MANAGER ---
    serial_puts("\n[1] TESTING MEMORY MANAGER (Must/Good/Bonus)\n");
    mem_init();
    void* a = malloc(1024); // Triggers splitting logic
    void* b = malloc(512);
    mem_status();           // Shows used/free status
    free(a);                // Triggers coalescing logic
    free(b);
    mem_status();

    // --- FEATURE: PROCESS MANAGER & IPC ---
    serial_puts("\n[2] TESTING PROCESS MANAGER & IPC (Must/Good/Bonus)\n");
    proc_init();
    pid_t p1 = proc_create(0, task_entry);
    
    // Demonstrate IPC Bonus
    proc_signal_register(7, demo_signal_handler);
    proc_signal_send(p1, 7, 0x1234);            
    
    // Show Process Table Utility
    proc_list();

    // --- FEATURE: SCHEDULER & AGING ---
    serial_puts("\n[3] TESTING SCHEDULER & AGING (Must/Good/Bonus)\n");
    scheduler_init();
    scheduler_set_quantum(10); // Demonstrate configurable quantum

    serial_puts("Starting Round Robin Multitasking...\n");
    
    /* Manual loop to simulate timer ticks and trigger aging */
    for(int i = 0; i < 110; i++) {
        scheduler_tick();
    }

    serial_puts("\nDemo Complete. Kernel Halting.\n");
    while(1);
}