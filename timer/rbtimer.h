#ifndef _RB_TIMER_H_
#define _RB_TIMER_H_

#include "ngx_rbtree.h"
#include <stdint.h>


static ngx_rbtree_node_t sentinel;
ngx_rbtree_t timer;


typedef struct timer_entry_s timer_entry_t;
typedef void (*timer_handler_ptr)(timer_entry_t* entry);


struct timer_entry_s {
    ngx_rbtree_node_t rbnode;
    timer_handler_ptr handler;
};


uint32_t current_time();

void init_timer();
void add_timer(uint32_t msec, timer_handler_ptr func);
void del_timer(timer_entry_t* entry);
void handle_timer();

uint32_t find_nearest_expire_timer();


#endif // _RB_TIMER_H_
