/*
 * os_task.c
 *
 *  Created on: 3 ago. 2022
 *      Author: ferna
 */

#include "os_task.h"
#include "os_core.h"
/*Funcion de comparacion que se manda a la funcion de ordanamiento para ordenar las tareas
por priorida donde 0 es la tarea con menor prioridad y el 4 con la mayor prioridad el 0 se le asigna a la tarea IDLE */
int compare_task(t_node* n1, t_node* n2)
{
	t_os_task* t1 = (t_os_task*)n1->data;
	t_os_task* t2 = (t_os_task*)n2->data;
    return t1->priority < t2->priority;
}

//Funcion de retardo
void delay_task(uint32_t ticks)
{
	//Si la funcion delay se llama de un interrupcion se da un error del sistema
	if (get_status_os()==MODE_IRQ)
	{
		error_hook();
	}
	t_os_task* temp_task;
	temp_task=get_task_current();
	if (ticks>0)
	{
		temp_task->ticks_bloked=ticks;
		temp_task->state=BLOKED;
	}
	os_yield();
}

/*Funcion  tick hook que es llamada cada tick del sistema implementada como WEAK para que el usuario
si desea la vuelva a definirr*/
void  weak tick_hook(void)
{
	__NOP();
}

/*Funcion de la tarea IDLE del sistema implementada como WEAK para que el usuario si desea utilizarala la
 * vuelva a definir, */
void weak idle_hook(void)
{
	while(1)
	{
		__WFI();
	}
}

/*Funcion del error hook utilizada para manejar errores del sistema  implementada como 	WEAK para
 * que el usuario la defina nuevamente si desea utilizarla*/
void  weak error_hook(void)
{
	while(1)
	{
	}
}
