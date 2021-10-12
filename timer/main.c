#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>
#include "rbtimer.h"

int g_running = 1;

void sighandler(int signo) {
    g_running = 0;
}

void hello_world(timer_entry_t* entry) {
    printf("timer expire key = %lu, now = %u\n", entry->rbnode.key, current_time());
}

int main(int argc, char* argv[]) {
    printf("timer begin running...\n");
    signal(SIGINT, sighandler);
    // int epoll_create(int size);
    // 创建一个epoll实例。其中nfd为epoll句柄，参数max_size标识这个监听的数目最大有多大，
    // 从Linux 2.6.8开始，max_size参数将被忽略，但必须大于零。
    int epfd = epoll_create(256);
    if (epfd == -1) {
        printf("epoll create failed\n");
        return -1;
    }
    struct epoll_event events[512];
    init_timer();
    add_timer(2000, hello_world);
    while(g_running) {
        // int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        // timeout 如果没有网络事件，最多等待多长时间
        uint32_t timeout = find_nearest_expire_timer();
        // printf("find nearest expire time = %u\n", timeout);
        int n = epoll_wait(epfd, events, sizeof(events), timeout);
        if (n == -1) {
            printf("epoll wait error\n");
            break;
        }
        for (int i = 0; i < n; i++) {
            // TODO: hander event
        }
        handle_timer();
    }
    close(epfd);
    printf("timer end running...\n");
    return 0;
}
