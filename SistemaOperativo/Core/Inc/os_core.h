#ifndef  OS_CORE_H
#define OS_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include "os_task.h"

typedef enum {
	NORMAL,
	RESETT
} e_state_os;

void os_control_add_task(task_function fn_task, void*parameter, uint8_t priority, t_os_task* task, t_node* node);
void os_scheduler(void);
void os_init(void);
uint32_t get_next_context(uint32_t sp_actual);

#endif
