#include "rbtimer.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


uint32_t current_time() {
    uint32_t t;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    t = (uint32_t)ts.tv_sec * 1000;
    t += ts.tv_nsec / 1000000;
    return t;
}


void init_timer() {
    ngx_rbtree_init(&timer, &sentinel, ngx_rbtree_insert_timer_value);
}

void add_timer(uint32_t msec, timer_handler_ptr func) {
    timer_entry_t* entry = (timer_entry_t*)malloc(sizeof(timer_entry_t));
    memset(entry, 0, sizeof(timer_entry_t));
    entry->handler = func;
    entry->rbnode.key = msec + current_time();
    ngx_rbtree_insert(&timer, &entry->rbnode);
}

void del_timer(timer_entry_t* entry) {
    ngx_rbtree_delete(&timer, &entry->rbnode);
    free(entry);
}

void handle_timer() {
    ngx_rbtree_node_t* node;
    for (;;) {
        if (timer.root == timer.sentinel) break;
        node = ngx_rbtree_min(timer.root, timer.sentinel);
        if (node->key > current_time()) break;
        timer_entry_t* entry = (timer_entry_t*)((char*)node - offsetof(timer_entry_t, rbnode));
        entry->handler(entry);
        printf("handle timer once\n");
        del_timer(entry);
    }
}

uint32_t find_nearest_expire_timer() {
    if (timer.root == timer.sentinel) {
        printf("rbtree is empty\n");
        return 200;
    }
    ngx_rbtree_node_t* node = ngx_rbtree_min(timer.root, timer.sentinel);
    uint32_t time = node->key - current_time();
    return time > 0 ? time : 200;
}

