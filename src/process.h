#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

#define MAX_PROCESSES 32
#define MAX_CHILDREN  8
#define MAX_SIGNALS   16
#define PROC_STACK_SIZE 4096

typedef enum {
    PROC_FREE = 0,
    PROC_CREATED,
    PROC_RUNNING,
    PROC_BLOCKED,
    PROC_ZOMBIE
} proc_state_t;

typedef struct process {
    pid_t    pid;           /* Changed to pid_t from types.h */
    pid_t    ppid;          /* Changed to pid_t from types.h */
    proc_state_t state;
    int32_t  exit_code;
    
    pid_t    children[MAX_CHILDREN];
    uint32_t child_count;
    
    /* Handler signature with 32-bit payload support */
    void (*signal_handlers[MAX_SIGNALS])(int, uint32_t);
    
    uint32_t *esp;          /* Saved stack pointer for context switch */
    uint8_t  *stack;        /* Allocated memory base */
    
    uint32_t cpu_ticks;     /* For scheduler aging */
} process_t;

/* Lifecycle */
void proc_init(void);
pid_t proc_create(pid_t ppid, void (*entry_point)(void));
void proc_exit(int32_t code);

/* State Management */
void proc_set_state(pid_t pid, proc_state_t state);
void proc_block(void);
void proc_wakeup(pid_t pid);

/* Utilities */
pid_t proc_getpid(void);
pid_t proc_getppid(void);
process_t* proc_get(pid_t pid);
void proc_list(void);

/* IPC */
void proc_signal_register(int sig, void (*handler)(int, uint32_t));
int32_t proc_signal_send(pid_t pid, int sig, uint32_t data);

#endif