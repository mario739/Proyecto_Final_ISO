

#ifndef OS_QUEUE_H
#define OS_QUEUE_H

#include <stdint.h>
#include <stddef.h>
#include "os_task.h"

#define QUEUE_LENGTH 10

typedef struct
{
	uint8_t* head_data;
	uint8_t* tail_data;
	uint8_t* write_data;
	t_os_task *task_asso;
	uint8_t count_items;
	uint8_t amount_items;
	uint8_t size_item;
}t_os_queue;

void init_queue(t_os_queue* queue, uint8_t* head, uint8_t amount_items, uint8_t size_item);
void queue_send(t_os_queue* queue, void* data);
void queue_receive(t_os_queue* queue, void* data);
#endif
