#ifndef  OS_TASK_H
#define OS_TASK_H

#include <stdint.h>

#define STACK_SIZE 256

#define XPSR			1				//Primer registro que se coloca en el stack siempre lleva uno en el bit 24
#define PC_REG		2				//Registro del contador del programa
#define LR			  		3				//Link register

/*Registros de proposito genereal*/
#define R12	    		4
#define R3			  		5
#define R2					6
#define R1					7
#define R0					8
#define LR_PREV_VALUE	9
#define R4				10
#define R5				11
#define R6				12
#define R7				13
#define R8				14
#define R9				15
#define R10 			16
#define R11 			17



typedef void (*task_function)( void * );

#define INIT_XPSR 	1 << 24							    //Siempre el bit 24 en 1

#define EXEC_RETURN	0xFFFFFFF9 				//valor que se tiene el EXEC_RETURN que se tiene que cargar al PC para que haga unstacking sin FPU

#define STACK_FRAME_SIZE		8			            //Cantidad de registros que se hace el stacking automatico
#define FULL_REG_STACKING_SIZE 		17	//16 core registers se adiciona un registro mas para guardar el lr

#define MAX_TASK_COUNT 8

typedef enum {
	RUNNING,
	READY,
}e_state_task;

typedef struct {
	uint32_t stack_task[STACK_SIZE/4];
	uint32_t stack_pointer;
	task_function task_pointer;
	e_state_task state;
	uint8_t priority;
	uint32_t ticks_bloked;
}t_os_task;


void os_task_create(task_function task, uint32_t *stack_task, uint32_t  *stack_pointer,void*parameter); //funcion para inicializar los punteros a las tareas

#endif
