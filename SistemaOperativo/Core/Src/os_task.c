/*
 * os_task.c
 *
 *  Created on: 3 ago. 2022
 *      Author: ferna
 */

#include "os_task.h"
#include "cmsis_gcc.h"

//Funcion para inicializar las tareas
void os_task_create(task_function task, t_os_task *t_task,void*parameter,uint8_t priority)
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
	t_task->state=READY;
}

void  weak tick_hook(void)
{
	__NOP();
}
void weak idle_hook(void)
{
	while(1)
	{
		__WFI();
	}
}

void  weak error_hook(void)
{
	while(1){
	}
}
