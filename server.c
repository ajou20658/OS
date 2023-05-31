#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LISTENQ 10
#define MAXLINE 1024

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    // time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("소켓 생성 실패");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("바인딩 실패");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) < 0) {
        perror("리스닝 실패");
        exit(1);
    }

    for (;;) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if (connfd < 0) {
            perror("연결 수락 실패");
            exit(1);
        }
        if ((listenfd == fork())==0){
            close(listenfd);

        }
        close(connfd);
    }

    return 0;
}
