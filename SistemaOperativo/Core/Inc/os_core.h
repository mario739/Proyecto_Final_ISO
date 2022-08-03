
#ifndef  OS_CORE_H
#define OS_CORE_H


#include <stdint.h>
#include <stdbool.h>
#include"os_task.h"

extern uint32_t sp_task1;
extern uint32_t sp_task2;
extern uint32_t sp_task3;

typedef enum {
	RUN,
	RESETT
}e_state_os;

typedef struct {
	void* list_task[MAX_TASK_COUNT];
	e_state_os state_os;
	bool scheduler_IRQ;
	uint32_t error;
	t_os_task* task_current;
	t_os_task* task_next;
}t_os_control;


void os_init(void);
uint32_t get_next_context(uint32_t sp_actual);
#endif
