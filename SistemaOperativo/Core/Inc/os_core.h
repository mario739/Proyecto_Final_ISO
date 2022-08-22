#ifndef  OS_CORE_H
#define OS_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include "os_task.h"
#include "os_semaphore.h"
typedef enum {
	NORMAL,
	RESETT
} e_state_os;

void os_control_add_task(task_function fn_task, void*parameter, uint8_t priority, t_os_task* task, t_node* node);
t_os_task* get_task_current(void);
void update_time_delay(void);
void os_scheduler(void);
void os_yield(void);
void set_pendSV(void);
void os_init(void);
void os_enter_critical(void);
void os_exit_critical(void);
uint32_t get_next_context(uint32_t sp_actual);

#endif
