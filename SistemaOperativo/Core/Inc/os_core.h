
#ifndef  OS_CORE_H
#define OS_CORE_H


#include <stdint.h>
#include <stdbool.h>
#include"os_task.h"

extern t_os_task t_task1;
extern t_os_task t_task2;
extern t_os_task t_task3;

typedef enum {
	NORMAL,
	RESETT
}e_state_os;


void os_init(void);
void os_task_create(task_function task, t_os_task *t_task,void*parameter,uint8_t priority);
uint32_t get_next_context(uint32_t sp_actual);

#endif
