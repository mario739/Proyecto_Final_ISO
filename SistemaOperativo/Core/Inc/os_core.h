
#ifndef  OS_CORE_H
#define OS_CORE_H


#include <stdint.h>

#define STACK_SIZE 256		//tamaño del stack de las tareas


#define XPSR			1				//Primer registro que se coloca en el stack siempre lleva uno en el bit 24
#define PC_REG		2				//Registro del contador del programa
#define LR			  		3				//Link register

//Registros de proposito general
#define R12	    		4
#define R3			  		5
#define R2					6
#define R1					7
#define R0					8


#define INIT_XPSR 	1 << 24		//Siempre el bit 24 en 1

#define EXEC_RETURN	0xFFFFFFF9 	//valor que se tiene el EXEC_RETURN que se tiene que cargar al PC para que haga unstacking sin FPU

#define STACK_FRAME_SIZE		8			//Cantidad de registros que se hace el stacking automatico
#define FULL_REG_STACKING_SIZE 		16	//16 core registers

void os_init_task(void * task, uint32_t *stack_task, uint32_t  *stack_pointer); //funcion para inicializar los punteros a las tareas


#endif
