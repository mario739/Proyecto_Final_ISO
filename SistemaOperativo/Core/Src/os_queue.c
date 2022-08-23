/*
 * os_queue.c
 *
 *  Created on: 20 ago. 2022
 *      Author: ferna
 */
#include "os_queue.h"

/*Funcion de inicializacion de una queue*/
void init_queue(t_os_queue* queue, uint8_t* head, uint8_t amount_items, uint8_t size_item){
    queue->head_data = head;
    queue->write_data = head;
    queue->tail_data = head + (amount_items*size_item);
    queue->amount_items = amount_items;
    queue->size_item = size_item;
}

/*Funcion para verificar si la queue esta llena*/
uint8_t is_queue_full(t_os_queue* queue){
    if(queue->write_data == queue->tail_data){
        return 1;
    }
    return 0;
}

/*Funcion para verificar si la queue esta basia*/
uint8_t is_queue_empty(t_os_queue* queue){
    if(queue->write_data == queue->head_data){
        return 1;
    }
    return 0;
}
/*Funcion para enviar datos a la queue */
void queue_send(t_os_queue* queue, void* data)
{
	 t_os_task *task_current;
	 if(is_queue_full(queue) == 0)
		{
			memcpy(queue->write_data, data, queue->size_item);
			queue->write_data += queue->size_item;
			queue->count_items++;
			if (queue->task_asso!=NULL)
			{
				queue->task_asso->state=READY;
				if (get_status_os()==MODE_IRQ)
				{
					set_status_scheduler_irq(true);
				}
				else
				{
						os_yield();
				}

			}
		}
		else
		{
			//Si se quiere escribir a una cola llena desde un irq no bloqueara por que es una irq
			if (get_status_os()!=MODE_IRQ)
			{
				//Se agrega un seccion critica al momento de obtener la tarea actual y cambiar su estado
				os_enter_critical();
				task_current=get_task_current();
				queue->task_asso=task_current;
				task_current->state=BLOKED;
				os_exit_critical();
				os_yield();
			}
		}
}

/*Funcion para recivir datos por la queue */
void queue_receive(t_os_queue* queue, void* data)
{
	t_os_task *task_current;
	if(is_queue_empty(queue) == 0)
    {
		memcpy(data, queue->head_data, queue->size_item);
	    queue->write_data -= queue->size_item;
		queue->count_items--;
	    memcpy(queue->head_data, queue->head_data + queue->size_item, queue->size_item * queue->count_items);
	    if (queue->task_asso!=NULL)
	    {
	    	 queue->task_asso->state=READY;
	    	 //Si se llamo de una irq se setea el bit para posteriormente hacer un resqueduling
	    	 if (get_status_os()==MODE_IRQ) {
				set_status_scheduler_irq(true);
			}
	    	 else
	    	 {
	    		 os_yield();
	    	 }

		}
    }
	else
	{
		//Si se quiere leer de un cola basia en modo irq no se bloquea por que es una interrupcion
		if (get_status_os()!=MODE_IRQ)
		{
			//Se agrega un seccion critica al momento de obtener la tarea actual y cambiar su estado
			os_enter_critical();
			task_current=get_task_current();
			queue->task_asso=task_current;
			task_current->state=BLOKED;
			os_exit_critical();
			os_yield();
		}

	}
}
//Funcion para obtener la catidad de elementos en la que se encuentra todavia en la queue
uint8_t get_queue_count(t_os_queue* queue)
{
	if (queue!=NULL)
	{
		return queue->count_items;
	}
	return 0;
}
