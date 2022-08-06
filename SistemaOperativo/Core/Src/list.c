/*
 * list.c
 *
 *  Created on: Aug 5, 2022
 *      Author: ferna
 */
#include <stdio.h>
#include "list.h"

static void add_node_at_front(t_list* list, t_node* node){
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

static void add_node_at_back(t_list* list, t_node* node){
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void add_node(t_list* list, t_node* node, enum_list_position position){
    if(NULL == list->head){
        list->head = node;
        list->tail = node;
    }else if(position == FRONT){
        add_node_at_front(list, node);
    }else if(position == BACK){
        add_node_at_back(list, node);
    }
    list->size++;
}

t_node* get_node(t_list* list, uint8_t id){
    t_node* temp_head = list->head;
    while(NULL != temp_head){
        if(temp_head->id == id){
            return temp_head;
        }
        temp_head = temp_head->next;
    }
    return NULL;
}

t_node* remove_node(t_list* list, uint8_t id){
    t_node* temp_head = list->head;
    while(NULL != temp_head){
        if(temp_head->id == id){
        	if(NULL == temp_head->prev &&
        			NULL == temp_head->next){
        		list->head = NULL;
        		list->tail = NULL;
        	}
        	else if(NULL == temp_head->prev){
            	temp_head->next->prev = NULL;
            	list->head = temp_head->next;
            }else if(NULL == temp_head->next){
            	temp_head->prev->next =  NULL;
            	list->tail = temp_head->prev;
            }else{
            	temp_head->prev->next = temp_head->next;
            }
            list->size--;
        	//list->head=list->head->next;
            //temp_head->prev=NULL;
            //temp_head->next=temp_head->next->next;
            temp_head->next = NULL;
            temp_head->prev = NULL;

            return temp_head;
        }
        temp_head = temp_head->next;
    }
    return NULL;
}

static void swap_nodes(t_node* n1, t_node* n2){
    t_node* prev = n1->prev;

    n1->next = n2->next;
    n1->prev = n2;
    if(NULL != n1->next){
        n1->next->prev = n1;
    }

    n2->prev = prev;
    n2->next = n1;
    if(NULL != n2->prev){
        n2->prev->next = n2;
    }
}

void sort(t_list* list, fn_compare_nodes compare_nodes){
    t_node* head = list->head;
    t_node* tail = list->tail;
    t_node* current;

    for (uint8_t i = 0; i < list->size; i++)
    {
        current = head;
        while(NULL != current->next){
            if(compare_nodes(current, current->next) == 1){
                if(NULL == current->prev){
                    head = current->next;
                }
                if(NULL == current->next->next){
                    tail = current;
                }
                swap_nodes(current, current->next);
            }else{
                current = current->next;
            }
        }
    }
    list->head = head;
    list->tail = tail;
}

void init_list(t_list* list)
{
	list->head=NULL;

}
