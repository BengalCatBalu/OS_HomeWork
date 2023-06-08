#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define THE_END "The End"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, new_socket1, new_socket2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUF_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket1 = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if ((new_socket2 = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t n = recv(new_socket1, buffer, BUF_SIZE, 0);
        if (n <= 0 || strcmp(buffer, THE_END) == 0) {
            send(new_socket2, THE_END, strlen(THE_END), 0);
            break;
        }
        send(new_socket2, buffer, n, 0);
    }

    close(new_socket1);
    close(new_socket2);
    close(server_fd);

    return 0;
}
