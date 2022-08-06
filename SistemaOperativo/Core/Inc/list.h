#ifndef LIST
#define LIST

#include <stdint.h>

typedef enum {
    FRONT,
    BACK
} enum_list_position;

typedef struct s_node {
    struct s_node* prev;
    struct s_node* next;
    uint8_t id;
    void* data;
} t_node;

typedef struct s_list {
    struct s_node* head;
    struct s_node* tail;
    uint8_t size;
} t_list;

typedef int (*fn_compare_nodes)(t_node*, t_node*);

void add_node(t_list*, t_node*, enum_list_position);

t_node* get_node(t_list*, uint8_t);

t_node* remove_node(t_list*, uint8_t);

void sort(t_list*, fn_compare_nodes);

#endif
