/*
 * os_core.c
 *
 *  Created on: Jul 27, 2022
 *      Author: ferna
 */
#include "os_core.h"
#include "main.h"



#define MAX_TASK_COUNT 8


typedef struct {
	void* list_task[MAX_TASK_COUNT];
	e_state_os state_os;
	bool scheduler_IRQ;
	uint32_t error;
	t_os_task* task_current;
	t_os_task* task_next;
}t_os_control;

static t_os_control os_control;
t_os_task t_task_idle;

void os_init(void)
{
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);  //Seteamos la prioridad mas baja al la excepcion de PendSV

	os_control.error=0;
	os_control.state_os=RESETT;
	os_control.task_current=NULL;
	os_control.task_next=NULL;

	for (uint8_t i = 0; i < MAX_TASK_COUNT; i++)  {
			os_control.list_task[i] = NULL;
	}

	 os_task_create(idle_hook,&t_task_idle,(void*)1,1);
}

uint32_t get_next_context(uint32_t sp_actual)  {
	static int32_t tarea_actual = -1;
	uint32_t sp_siguiente;

	switch(tarea_actual)  {

	case 1:
		t_task1.stack_pointer = sp_actual;
		sp_siguiente = t_task2.stack_pointer;
		tarea_actual = 2;
		break;

	case 2:
		 t_task2.stack_pointer = sp_actual;
		sp_siguiente =  t_task3.stack_pointer;
		tarea_actual = 3;
		break;

	case 3:
		 t_task3.stack_pointer = sp_actual;
		sp_siguiente =  t_task_idle.stack_pointer;
		tarea_actual = 4;
		break;
	case 4:
		 t_task_idle.stack_pointer = sp_actual;
		sp_siguiente =  t_task1.stack_pointer;
		tarea_actual = 1;
	default:
		sp_siguiente =  t_task1.stack_pointer;
		tarea_actual = 1;
		break;
	}
	return sp_siguiente;
}
