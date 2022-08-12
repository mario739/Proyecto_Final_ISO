/*
 * os_semaphore.c
 *
 *  Created on: Aug 12, 2022
 *      Author: ferna
 */

#include "os_semaphore.h"

void semaphore_init(t_semaphore* semaphore)
{
	semaphore->state=TAKE;
	semaphore->task_associated=(void*)0;
}
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
void semaphore_give(t_semaphore *semaphore)
{
	t_os_task* temp_task;
	temp_task=get_task_current();
	if (semaphore->state==TAKE)
	{
		semaphore->state=GIVE;
		semaphore->task_associated->state=READY;
		//os_yield();
	}
}
