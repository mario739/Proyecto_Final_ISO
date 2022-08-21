/*
 * os_queue.c
 *
 *  Created on: 20 ago. 2022
 *      Author: ferna
 */
#include "os_queue.h"

void init_queue(t_os_queue* queue, uint8_t* head, uint8_t amount_items, uint8_t size_item){
    queue->head_data = head;
    queue->write_data = head;
    queue->tail_data = head + (amount_items*size_item);
    queue->amount_items = amount_items;
    queue->size_item = size_item;
}

uint8_t is_queue_full(t_os_queue* queue){
    if(queue->write_data == queue->tail_data){
        return 1;
    }
    return 0;
}

uint8_t is_queue_empty(t_os_queue* queue){
    if(queue->write_data == queue->head_data){
        return 1;
    }
    return 0;
}

void queue_send(t_os_queue* queue, void* data){
    if(is_queue_full(queue) == 0){
        memcpy(queue->write_data, data, queue->size_item);
        queue->write_data += queue->size_item;
        queue->count_items++;
    }
}

void queue_receive(t_os_queue* queue, void* data){
    if(is_queue_empty(queue) == 0){
        memcpy(data, queue->head_data, queue->size_item);
        queue->write_data -= queue->size_item;
        queue->count_items--;
        memcpy(queue->head_data, queue->head_data + queue->size_item, queue->size_item * queue->count_items);
    }
}
