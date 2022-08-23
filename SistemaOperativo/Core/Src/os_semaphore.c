/*
 * os_semaphore.c
 *
 *  Created on: Aug 12, 2022
 *      Author: ferna
 */

#include "os_semaphore.h"
#include "os_core.h"

//Funcion para inicializar un semaforo binario
void semaphore_init(t_semaphore* semaphore)
{
	semaphore->state=TAKE;
	semaphore->task_associated=(void*)0;
}
//Funcion para tomar un semaforo binario,si el semaforo esta tomado la tarea se bloquea
void semaphore_take(t_semaphore* semaphore)
{
	t_os_task* temp_task;
	temp_task=get_task_current();
	if (semaphore->state==TAKE)
	{
		temp_task->state=BLOKED;
		semaphore->task_associated=temp_task;
		os_yield();
	}
		semaphore->state=TAKE;
}
//Funcion para dar un semaforo, si habia una tarea bloqueada en espera del semaforo esta pasa a READY
void semaphore_give(t_semaphore *semaphore)
{
	if (semaphore->state==TAKE)
	{
		semaphore->state=GIVE;
		semaphore->task_associated->state=READY;
		if (get_status_os()==MODE_IRQ) {
			set_status_scheduler_irq(true);
		}
	}
}
