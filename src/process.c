#include "process.h"
#include "memory.h"
#include "serial.h"
#include "string.h"

static process_t procs[MAX_PROCESSES];
static pid_t next_pid = 1;      /* Using pid_t for ID generation */
static pid_t current_pid = 0;   /* Tracks the active process index/ID */

void proc_init(void) {
    memset(procs, 0, sizeof(procs));

    for (int i = 0; i < MAX_PROCESSES; i++) {
        procs[i].ppid = -1;
    }

    /* Process 0: kernel/init */
    procs[0].pid = 0;
    procs[0].state = PROC_RUNNING;
    current_pid = 0;

    serial_puts("[PROC] Manager initialized\n");
}

pid_t proc_create(pid_t ppid, void (*entry_point)(void)) {
    int i;
    for (i = 1; i < MAX_PROCESSES; i++) {
        if (procs[i].state == PROC_FREE) break;
    }
    if (i == MAX_PROCESSES) return -1;

    process_t *p = &procs[i];
    p->pid = next_pid++;
    p->ppid = ppid;
    p->state = PROC_CREATED;
    p->cpu_ticks = 0;

    /* Allocate process stack */
    p->stack = (uint8_t*)malloc(PROC_STACK_SIZE);
    if (!p->stack) return -1;

    /* x86 Stack Frame Setup for context switch */
    uint32_t *stack_top = (uint32_t*)((uint8_t*)p->stack + PROC_STACK_SIZE);
    *(--stack_top) = (uintptr_t)entry_point; /* Using uintptr_t for pointer safety */
    *(--stack_top) = 0; /* EBP */
    *(--stack_top) = 0; /* EDI */
    *(--stack_top) = 0; /* ESI */
    *(--stack_top) = 0; /* EBX */
    p->esp = stack_top;

    /* Register as child of parent */
    if (ppid >= 0 && ppid < MAX_PROCESSES) {
        process_t *parent = proc_get(ppid);
        if (parent && parent->child_count < MAX_CHILDREN) {
            parent->children[parent->child_count++] = p->pid;
        }
    }

    serial_puts("[PROC] Created PID: "); 
    serial_print_u32(p->pid); 
    serial_puts(" (State: READY)\n"); /* For demo clarity */
    return p->pid;
}

void proc_set_state(pid_t pid, proc_state_t state) {
    process_t *p = proc_get(pid);
    if (p) p->state = state;
}

void proc_block(void) {
    procs[current_pid].state = PROC_BLOCKED;
    serial_puts("[PROC] PID "); 
    serial_print_u32(procs[current_pid].pid);
    serial_puts(" transitioned to BLOCKED\n"); /* For demo */
}

void proc_wakeup(pid_t pid) {
    process_t *p = proc_get(pid);
    if (p && p->state == PROC_BLOCKED) {
        p->state = PROC_CREATED; /* Effectively READY */
        serial_puts("[PROC] PID "); 
        serial_print_u32(pid);
        serial_puts(" unblocked (READY)\n"); /* For demo */
    }
}

void proc_exit(int32_t code) {
    procs[current_pid].exit_code = code;
    procs[current_pid].state = PROC_ZOMBIE;
    
    /* Clean up stack memory */
    if (procs[current_pid].stack) {
        free(procs[current_pid].stack);
        procs[current_pid].stack = NULL;
    }

    serial_puts("[PROC] PID "); 
    serial_print_u32(procs[current_pid].pid); 
    serial_puts(" exited code "); 
    serial_print_u32((uint32_t)code); 
    serial_puts(" (State: ZOMBIE)\n"); /* For demo */
}

pid_t proc_getpid(void) { return procs[current_pid].pid; }

pid_t proc_getppid(void) { return procs[current_pid].ppid; }

process_t* proc_get(pid_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].pid == pid) return &procs[i];
    }
    return NULL;
}

void proc_list(void) {
    serial_puts("\n--- PROCESS TABLE STATUS ---\n");
    serial_puts("PID\tPPID\tSTATE\t\tTICKS\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (procs[i].state != PROC_FREE) {
            serial_print_u32(procs[i].pid); serial_puts("\t");
            if (procs[i].ppid == -1) serial_puts("-1"); 
            else serial_print_u32(procs[i].ppid);
            serial_puts("\t");
            
            switch(procs[i].state) {
                case PROC_RUNNING: serial_puts("RUNNING\t\t"); break;
                case PROC_BLOCKED: serial_puts("BLOCKED\t\t"); break;
                case PROC_ZOMBIE:  serial_puts("ZOMBIE\t\t");  break;
                case PROC_CREATED: serial_puts("READY\t\t");   break;
                default:           serial_puts("UNKNOWN\t\t"); break;
            }
            serial_print_u32(procs[i].cpu_ticks);
            serial_puts("\n");
        }
    }
    serial_puts("----------------------------\n");
}

/* IPC: Signal registration */
void proc_signal_register(int sig, void (*handler)(int, uint32_t)) {
    if (sig >= 0 && sig < MAX_SIGNALS) {
        procs[current_pid].signal_handlers[sig] = handler;
        serial_puts("[IPC] Registered handler for Signal "); /* Demo log */
        serial_print_u32((uint32_t)sig);
        serial_puts("\n");
    }
}

/* IPC: Signal transmission */
int32_t proc_signal_send(pid_t pid, int sig, uint32_t data) {
    process_t *p = proc_get(pid);
    if (p && sig >= 0 && sig < MAX_SIGNALS && p->signal_handlers[sig]) {
        serial_puts("[IPC] Sending Signal "); /* For demo */
        serial_print_u32((uint32_t)sig);
        serial_puts(" to PID "); 
        serial_print_u32(pid); 
        serial_puts(" with data payload\n");
        p->signal_handlers[sig](sig, data);
        return 0;
    }
    serial_puts("[IPC] ERROR: Signal delivery failed\n"); /* Error log */
    return -1;
}