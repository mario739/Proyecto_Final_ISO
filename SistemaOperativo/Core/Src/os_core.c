/*
 * os_core.c
 *
 *  Created on: Jul 27, 2022
 *      Author: ferna
 */
#include "os_core.h"
#include "main.h"

#define MAX_TASK_COUNT 8	 //Cantidad maximo de tareas
#define NUMBER_OF_LISTS 2	//

//Estrucutura principal del SO
typedef struct {
	t_os_task* list_task[MAX_TASK_COUNT];
	t_list list_core[NUMBER_OF_LISTS];
	e_state_os state_os;
	task_function idle_task;
	bool scheduler_IRQ;
	uint8_t amount_task;
	uint32_t error;
	t_os_task* task_current;
	t_os_task* task_next;
} t_os_control;

//Creacion de la estructura principal del sistema operativo estatica para que no se tenga acceso fuera del archivo
static t_os_control os_control;

//Variables que utiliza el sistema operativo
t_os_task task_idle;
t_node node_idle;
t_list* list_ready = &os_control.list_core[0];
t_list* list_bloked = &os_control.list_core[1];

static void clean_list(t_list* list);

//Funcion para inicializar las variables de la estrucutra princiapal del SO
void initialize_os(){
	os_control.error = 0;
	os_control.state_os = RESETT;
	os_control.task_current = NULL;
	os_control.task_next = NULL;
}

void os_init(void)
{
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);  //Seteamos la prioridad mas baja al la excepcion de PendSV
    //Inicializamos la estructura del SO
	initialize_os();
	//Inicializamos el la para IDLE para el SO
	os_control_add_task(idle_hook, (void*)1, TASK_IDLE_PRIORITY, &task_idle, &node_idle);
	os_control.idle_task = idle_hook;
	//Ordenamos la lista de las tarea READY por su prioridad
    clean_list(list_ready);
	sort(list_ready, compare_task);

}

void load_stack_task(t_os_task* task,  task_function fn_task, void* parameter){
	task->stack[STACK_SIZE/4 - XPSR] = INIT_XPSR;					//Se carga en el registro 1 de stack el XPRS
	task->stack[STACK_SIZE/4 - PC_REG] = (uint32_t)fn_task;			//Se carga la dirrecion de la tarea a ejecutar en el PC
	task->stack[STACK_SIZE/4 - LR_PREV_VALUE] = EXEC_RETURN;  //Se carga el valor del EXEC_RETURN por que este valor se modifica en el asm al llmar a una funcion
	task->stack[STACK_SIZE/4 - STACK_FRAME_SIZE] = (uint32_t)parameter; //Se coloca el parametro que se desea pasar al registro R0
	task->stack_pointer =(uint32_t) (task->stack+ STACK_SIZE/4 - FULL_REG_STACKING_SIZE ); //El puntero al stack queda en la posicion 17 en el stack
}

//Funcion para inicializar las tareas
void initialize_task(t_os_task* task, task_function fn_task, void*parameter, uint8_t priority)
{
	if (NULL!=task)
	{
		//Se cargan valores necesarios a los registros para que el sistema funcione correctamente
			load_stack_task(task, fn_task, parameter);

		//Se carga los campos de estructura con los parametros pasados a la funcion
			task->task_pointer = fn_task;
			task->parameter = parameter;
			task->priority = priority;
			task->state=READY;
			task->ticks_bloked=-1;
	}
	else
	{
		error_hook();
	}
}

void initialize_node(t_node * node, t_os_task* task){
	static uint8_t i = 0;//contador de nodos inicilizados

	//Se inicializan los nodos de la lista
	node->next = NULL;
	node->prev = NULL;
	node->id = i;
	node->data = (void*)task;

	i++;
}

void os_control_add_task(task_function fn_task, void*parameter, uint8_t priority, t_os_task* task, t_node* node){
	//Se iniciliaza la tarea
	os_control.amount_task++;
	initialize_task(task, fn_task, parameter, priority);
	//Se inicializa el nodo de la tarea
	initialize_node(node, task);
	//Se adiciona el nodo a la lista de tareas ready
	add_node(list_ready, node,BACK);
}

//Funcion para asignar la nueva tarea a ejecutar a la estructura del OS
void os_control_add_next(t_node* node){
	os_control.task_next =  (t_os_task *)node->data;
	//os_control.task_next->state=RUNNING;

}
//Funcion para asignar la tarea actual a la estructura del OS
void os_control_add_current(t_node* node){
	os_control.task_current = (t_os_task *)node->data;
	//os_control.task_current->state=READY;
}

static void clean_list(t_list* list)
{
    t_node* temp_head = list->head;
    t_node*node=NULL;
    t_node*node2=NULL;
    while(NULL != temp_head)
    {
    	t_os_task* temp_task= (t_os_task *)temp_head->data;
    	if (list==list_ready)
    	{
            if(temp_task->state==BLOKED)
            {
            	node2=temp_head->next;
            	node=remove_node(list_ready, temp_head->id);
            	add_node(list_bloked, node, BACK);
            	temp_head=node2;
            }
            else
            {
            	temp_head = temp_head->next;
            }
		}
    	else if(list==list_bloked)
    	{
    		if(temp_task->state==READY)
				{
					node2=temp_head->next;
					node=remove_node(list_bloked, temp_head->id);
					add_node(list_ready, node, BACK);
					temp_head=node2;
				}
				else
				{
					temp_head = temp_head->next;
				}
		}
    }
}

void update_time_delay(void)
{
	t_node* temp_head = list_bloked->head;
	while(NULL != temp_head)
	{
		t_os_task* temp_task= (t_os_task *)temp_head->data;
		if (temp_task->ticks_bloked>0 )
		{
			temp_task->ticks_bloked--;
		}
		else if (temp_task->ticks_bloked==0 )
		{
			temp_task->state=READY;
			temp_task->ticks_bloked=-1;
		}
		temp_head = temp_head->next;
	}
}
//Manejador del sistema operativo
void os_scheduler(void)
{
	static t_node* node = NULL;

	if (os_control.state_os == RESETT)
	{
		node = remove_node(list_ready, list_ready->head->id);
		os_control_add_next(node);
	}
	else
	{
		if(NULL != node)
		{
			add_node(list_ready, node, BACK);
			clean_list(list_ready);
			clean_list(list_bloked);
			sort(list_ready, compare_task);
			os_control_add_current(node);

			node = remove_node(list_ready, list_ready->head->id);
			os_control_add_next(node);
		}
		else
		{
			error_hook();
		}
	}
}
//Funcion para obtener el nuevo contexto del sistema
uint32_t get_next_context(uint32_t sp_current)
{
	if (os_control.state_os ==RESETT)
	{
		os_control.state_os =NORMAL;;
	}
	else
	{
		os_control.task_current->stack_pointer = sp_current;
	}
	return os_control.task_next->stack_pointer;
}

t_os_task* get_task_current(void)
{
	return os_control.task_next;
}

void set_pendSV(void)
{
	 SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; //Se activa la excepcion del PendSV

	  __ISB();	//Limpia el pipeline y asugura que todas las instruciones se ejecuten

	  __DSB(); //Asegura que todos los accessos a memoria se ayan realizado antes de la siguiente instruccion

}
void os_yield(void)
{
	os_scheduler();
	set_pendSV();
}

inline void os_enter_critical(void)
{
	__disable_irq();
}

inline void os_exit_critical(void)
{
	__enable_irq();
}

