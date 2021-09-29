#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
static const int PORT = 9995;

int main(int argc, char* argv[]) {
    printf("say hello\n");
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        printf("create socket failed!\n");
        goto error;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(PORT);
    if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("connect failed!\n");
        goto error;
    }

    char recvbuf[BUFFER_SIZE] = {0};
    if (recv(sfd, recvbuf, BUFFER_SIZE, 0) < 0) {
        printf("recv error!\n");
        goto error;
    }
    printf("recv data: %s\n", recvbuf);

error:
    close(sfd);
    return 0;
}
