/*
 * os_core.c
 *
 *  Created on: Jul 27, 2022
 *      Author: ferna
 */
#include "os_core.h"
#include "main.h"


void os_init(void)
{
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);  //Seteamos la prioridad mas baja al la excepcion de PendSV
}

uint32_t get_next_context(uint32_t sp_actual)  {
	static int32_t tarea_actual = -1;
	uint32_t sp_siguiente;

	switch(tarea_actual)  {

	case 1:
		sp_task1 = sp_actual;
		sp_siguiente = sp_task2;
		tarea_actual = 2;
		break;

	case 2:
		sp_task2 = sp_actual;
		sp_siguiente = sp_task3;
		tarea_actual = 3;
		break;

	case 3:
		sp_task3 = sp_actual;
		sp_siguiente = sp_task1;
		tarea_actual = 1;
		break;

	default:
		sp_siguiente = sp_task1;
		tarea_actual = 1;
		break;
	}
	return sp_siguiente;
}
