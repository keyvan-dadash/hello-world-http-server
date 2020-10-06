#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <net/ethernet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <memory.h>


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd <= 0) {
        perror("create socket faced to error\n");
        return -1;
    }

    struct sockaddr_in servAddr;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(8080);


    if (bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("cannot bind socket");
        return -1;
    }

    if (listen(fd, 5) < 0) {
        perror("cannot listern on fd");
        return -1;
    }

    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    char buf[30000];

    while (1) {
        struct sockaddr_in client;

        socklen_t len = sizeof(client);

        int clientFd = accept(fd, (struct sockaddr *)&client, &len);

        memset(buf, '\0', strlen(buf));

        read(clientFd, buf, 30000);

        printf("%s\n", buf);

        write(clientFd, hello, strlen(hello));

        close(clientFd);

    }


}