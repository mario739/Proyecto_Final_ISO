#ifndef OS_SEMAPHORE_H
#define OS_SEMAPHORE_H

#include <stdbool.h>
#include "os_task.h"
#include "os_core.h"

typedef enum
{
	TAKE,
	GIVE
}e_semaphore_state;

typedef struct
{
	t_os_task* task_associated;
	e_semaphore_state state;
}t_semaphore;

void semaphore_init(t_semaphore* semaphore);
void semaphore_take(t_semaphore* semaphore);
void semaphore_give(t_semaphore* semaphore);
#endif
