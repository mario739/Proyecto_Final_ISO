
#ifndef  OS_CORE_H
#define OS_CORE_H


#include <stdint.h>
#include <stdbool.h>
#include"os_task.h"

extern t_os_task t_task1;
extern t_os_task t_task2;
extern t_os_task t_task3;

typedef enum {
	RUN,
	RESETT
}e_state_os;

void os_init(void);
uint32_t get_next_context(uint32_t sp_actual);
#endif
