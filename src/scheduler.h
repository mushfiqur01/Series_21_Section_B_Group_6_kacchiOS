#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"
#include "process.h"

#define DEFAULT_QUANTUM 10
#define MAX_TICKS_BEFORE_AGING 100

void scheduler_init(void);
void schedule(void);

/* Runtime configuration for the time quantum */
void scheduler_set_quantum(uint32_t q);

/* This handles periodic logic and the aging threshold check */
void scheduler_tick(void);

/* Low-level assembly function to swap CPU register states */
extern void switch_to(uint32_t **old_esp, uint32_t *new_esp);

#endif