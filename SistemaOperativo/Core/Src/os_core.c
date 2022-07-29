/*
 * os_core.c
 *
 *  Created on: Jul 27, 2022
 *      Author: ferna
 */
#include "os_core.h"

void os_init_task(void *task, uint32_t *stack_task, uint32_t  *stack_pointer)
{
	stack_task[STACK_SIZE/4 - XPSR] = INIT_XPSR;				//Se carga en el registro 1 de stack el XPRS
	stack_task[STACK_SIZE/4 - PC_REG] = (uint32_t)task;		//Se carga la dirrecion de la tarea a ejecutar en el PC
	*stack_pointer = (uint32_t) (stack_task+ STACK_SIZE/4 - FULL_REG_STACKING_SIZE );		//El puntero al stack queda en la posicion 8 en el stack
}

