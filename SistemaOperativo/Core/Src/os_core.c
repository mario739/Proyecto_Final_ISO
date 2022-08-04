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
	t_os_task* list_task[MAX_TASK_COUNT];
	e_state_os state_os;
	task_function idle_task;
	bool scheduler_IRQ;
	uint8_t amount_task;
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

	 os_task_create(idle_hook,&t_task_idle,(void*)1,TASK_IDLE_PRIORITY);
	 os_control.idle_task=idle_hook;
}

//Funcion para inicializar las tareas
void os_task_create(task_function task, t_os_task *t_task,void*parameter,uint8_t priority)
{
	static uint8_t i=0;
	if (NULL!=task)
	{
		t_task->stack[STACK_SIZE/4 - XPSR]= INIT_XPSR;					//Se carga en el registro 1 de stack el XPRS
		t_task->stack[STACK_SIZE/4 - PC_REG]= (uint32_t)task;			//Se carga la dirrecion de la tarea a ejecutar en el PC
		t_task->stack[STACK_SIZE/4 - LR_PREV_VALUE]= EXEC_RETURN;  //Se carga el valor del EXEC_RETURN por que este valor se modifica en el asm al llmar a una funcion
		t_task->stack[STACK_SIZE/4 - STACK_FRAME_SIZE]=(uint32_t)parameter; //Se coloca el parametro que se desea pasar al registro R0
		t_task->stack_pointer= (uint32_t) (t_task->stack+ STACK_SIZE/4 - FULL_REG_STACKING_SIZE ); //El puntero al stack queda en la posicion 17 en el stack

		//Se carga los campos de estructura con los parametros pasados a la funcion
		t_task->task_pointer=task;
		t_task->parameter=parameter;
		t_task->priority=priority;


		if (i==2)
		{
			t_task->state=BLOKED;
		}
		else
		{
			t_task->state=READY;
		}
		//Se adiciona a la lista de tareas
		os_control.list_task[i]=t_task;
		os_control.amount_task++;
		i++;

	}
	else
	{
		error_hook();
	}
}

void os_scheduler(void)
{
	static uint8_t i=0;
	if (os_control.state_os==RESETT)
	{
		os_control.task_next=(t_os_task*)os_control.list_task[0]->stack_pointer;
		os_control.list_task[0]->state=RUNNING;
		os_control.task_current=os_control.list_task[0];
		i++;
	}
	else
	{
		while(i<=os_control.amount_task)
		{
				if (i==4)
				{
					if (os_control.list_task[i-os_control.amount_task]->state==READY)
					{
						os_control.list_task[i-1]->state=READY;
						os_control.task_next=(t_os_task*)os_control.list_task[0]->stack_pointer;
						os_control.list_task[0]->state=RUNNING;
						os_control.task_current=os_control.list_task[i-1];
						i=1;
						break;
					}
					else	i=1;
				}
				else
				{
					if (os_control.list_task[i]->state==READY)
					{
						os_control.list_task[i-1]->state=READY;
						os_control.task_next=(t_os_task*)os_control.list_task[i]->stack_pointer;
						os_control.list_task[i]->state=RUNNING;
						os_control.task_current=os_control.list_task[i-1];
						i++;
						break;
					}
					else i++;
	}
}
}
}
uint32_t get_next_context(uint32_t sp_current)
{
	if (os_control.state_os==RESETT)
	{
		os_control.state_os=NORMAL;
	}
	else
	{
		os_control.task_current->stack_pointer= sp_current;
	}
	return (uint32_t)os_control.task_next;
}
