/**
build:
g++ epoll-wait-deviation.cpp -o epoll-wait-deviation -g -O0
./epoll-wait-deviation
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/epoll.h>
 
int64_t get_current_time(){
    struct timeval now;
    int ret = gettimeofday(&now, NULL);
    assert(ret != -1);
 
    return now.tv_sec * 1000 + now.tv_usec / 1000;
}
 
int main(int argc, char** argv){
    int ep = epoll_create(1024);
    if(ep == -1){
        printf("create epoll error!\n"); exit(-1);
    }
    
    for(int i = 0; i < 2 * 60 * 60 * 1000; i += 1000){
        int64_t start = get_current_time();
        
        struct epoll_event events[1];
        if(epoll_wait(ep, events, 1, i) == -1){
            printf("wait epoll error!\n"); exit(-1);
        }
        
        int64_t end = get_current_time();
        printf("epoll wait deviation=%ld, timeout=%d (ms)\n", end - start - i, i);
    }
    
    close(ep);
    
    return 0;
}
