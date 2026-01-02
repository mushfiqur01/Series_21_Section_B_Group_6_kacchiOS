#include "scheduler.h"
#include "serial.h"

static uint32_t time_quantum;
static uint32_t current_ticks;

void scheduler_init(void) {
    time_quantum = DEFAULT_QUANTUM;
    current_ticks = 0;
    serial_puts("[SCHED] Round Robin initialized\n");
}

/* Configurable time quantum */
void scheduler_set_quantum(uint32_t q) {
    time_quantum = q;
    serial_puts("[SCHED] Quantum updated to: ");
    serial_print_u32(q);
    serial_puts("\n");
}

/* Aging */
static void apply_aging(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t *p = proc_get_by_index(i); 
        if (p && p->state == PROC_CREATED) {
            p->cpu_ticks++; 
            if (p->cpu_ticks > MAX_TICKS_BEFORE_AGING) {
                
                serial_puts("[SCHED] Aging: PID ");
                serial_print_u32(p->pid);
                serial_puts(" prioritized (waiting too long)\n");
            }
        }
    }
}

void schedule(void) {
    pid_t current_pid = proc_getpid();
    process_t *current_proc = proc_get(current_pid);
    
    int next_idx = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t *p = proc_get_by_index(i);
        if (p && p->state == PROC_CREATED && p->cpu_ticks > MAX_TICKS_BEFORE_AGING) {
            next_idx = i;
            p->cpu_ticks = 0;
            break; 
        }
    }

    if (next_idx == -1) {
        for (int i = 1; i <= MAX_PROCESSES; i++) {
            int idx = (current_pid + i) % MAX_PROCESSES;
            process_t *p = proc_get_by_index(idx);
            if (p && p->state == PROC_CREATED) {
                next_idx = idx;
                break;
            }
        }
    }

    if (next_idx != -1) {
        process_t *next_proc = proc_get_by_index(next_idx);
        
        if (current_proc->state == PROC_RUNNING) {
            current_proc->state = PROC_CREATED; 
        }
        next_proc->state = PROC_RUNNING;

        /* Context switch */
        serial_puts("[SCHED] Context Switch: PID "); /* For demo */
        serial_print_u32(current_pid);
        serial_puts(" -> PID ");
        serial_print_u32(next_proc->pid);
        serial_puts("\n");

        switch_to(&current_proc->esp, next_proc->esp);
    }
}

void scheduler_tick(void) {
    current_ticks++;
    if (current_ticks >= time_quantum) {
        current_ticks = 0;
        serial_puts("[SCHED] Quantum expired, running scheduler...\n"); /* For demo */
        apply_aging();
        schedule();
    }
}