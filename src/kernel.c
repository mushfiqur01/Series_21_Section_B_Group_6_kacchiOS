#include "serial.h"
#include "memory.h"
#include "process.h"
#include "scheduler.h"

/* Simple string comparison for our shell */
int kstrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

/* Signal handler for IPC Demonstration */
void demo_signal_handler(int sig, uint32_t data) {
    serial_puts("\n >>> [SIGNAL] Interrupt Handler Executed!");
    serial_puts("\n     [SIGNAL] Received Sig: "); serial_print_u32((uint32_t)sig);
    serial_puts(" | Payload: "); serial_print_hex(data);
    serial_puts("\n");
}

void task_entry() {
    while(1) {
        serial_puts(".");
        for(volatile int i = 0; i < 2000000; i++); 
    }
}

void print_banner() {
    serial_puts("\033[H\033[J");
    serial_puts("================================================\n");
    serial_puts("   kacchiOS 21B6 v1.0 - Bare Metal Kernel Booted     \n");
    serial_puts("================================================\n");
    serial_puts(" Systems Initialized: [SERIAL] [MEM] [PROC] [SCHED]\n");
}

void kernel_main() {
    serial_init();
    mem_init();
    proc_init();
    scheduler_init();

    print_banner();

    char cmd[64];
    while(1) {
        serial_puts("\nroot@kacchiOS:/# ");
        serial_gets(cmd, 64);

        if (kstrcmp(cmd, "help") == 0) {
            serial_puts("Available Commands:\n");
            serial_puts("  mem   - Run Memory Manager Test (Split/Coalesce)\n");
            serial_puts("  ipc   - Test Inter-Process Communication\n");
            serial_puts("  proc  - Show Process Table (PCB status)\n");
            serial_puts("  run   - Start Round Robin Multitasking Demo\n");
            serial_puts("  clear - Re-print OS Banner\n");
        } 
        else if (kstrcmp(cmd, "mem") == 0) {
            serial_puts("\n--- [MODULE: MEMORY MANAGER] ---\n");
            serial_puts("[STEP 1] Allocating blocks 'A' (1024) and 'B' (512)...\n");
            void* a = malloc(1024);
            void* b = malloc(512);
            serial_puts("Block A Address: "); serial_print_hex((uint32_t)a);
            serial_puts("\nBlock B Address: "); serial_print_hex((uint32_t)b);
            serial_puts("\n");
            mem_status();
            
            serial_puts("\n[STEP 2] Freeing blocks to trigger coalescing...\n");
            free(a);
            free(b);
            mem_status();
            serial_puts("--- [MEM TEST COMPLETE] ---\n");
        } 
        else if (kstrcmp(cmd, "ipc") == 0) {
            serial_puts("\n--- [MODULE: IPC & SIGNALS] ---\n");
            pid_t p1 = proc_create(0, task_entry);
            serial_puts("Task PID 1 created for signal target.\n");
            proc_signal_register(7, demo_signal_handler);
            serial_puts("Sending Signal 7 with Data 0x1234...\n");
            proc_signal_send(p1, 7, 0x1234);
            serial_puts("\n[EXPECTED ERROR ANALYSIS]:\n");
            serial_puts("Signal Delivery Failed because PID 1 has no registered handler.\n");
            serial_puts("In kacchiOS, handlers are process-specific and stored in the PCB.\n");
            serial_puts("Even though PID 0 knows how to handle Signal 7, PID 1 does not.\n");
            serial_puts("The Kernel correctly blocks this to prevent an invalid jump.\n");
            serial_puts("For PID 0 same signal should give 'payload'\n");
            proc_signal_send(0, 7, 0x1234);
            serial_puts("--- [IPC TEST COMPLETE] ---\n");
        } 
        else if (kstrcmp(cmd, "proc") == 0) {
            serial_puts("\n--- [CURRENT PROCESS TABLE] ---\n");
            proc_list();
        } 
        else if (kstrcmp(cmd, "run") == 0) {
            serial_puts("\n--- [MODULE: SCHEDULER] ---\n");
            serial_puts("Setting Quantum: 10. Starting multitasking...\n");
            serial_puts("Press Ctrl+C in terminal to terminate QEMU.\n\n");
            
            proc_create(0, task_entry);
            scheduler_set_quantum(10);
            
            // This loop simulates the "Heartbeat" of the OS switching tasks
            for(int i = 0; i < 500; i++) { // Run for 500 ticks
                scheduler_tick();
            }
            serial_puts("\n\n[SCHED] Demo slice finished. Returning to shell.\n");
        }
        else if (kstrcmp(cmd, "clear") == 0) {
            print_banner();
        }
        else if (cmd[0] != '\0') {
            serial_puts("Command '"); serial_puts(cmd); serial_puts("' not found.\n");
        }
    }
}

