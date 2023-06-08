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
    if (argc != 4) {
        printf("Usage: %s <IP> <Port> <Role>\n", argv[0]);
        printf("Role: sender or receiver\n");
        exit(EXIT_FAILURE);
    }

    int sock = 0;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE] = {0};
    int is_sender = strcmp(argv[3], "sender") == 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    if (is_sender) {
        while (1) {
            printf("Введите сообщение: ");
            fgets(buffer, BUF_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            send(sock, buffer, strlen(buffer), 0);

            if (strcmp(buffer, THE_END) == 0) {
                break;
            }
        }
    } else {
        while (1) {
            ssize_t n = recv(sock, buffer, BUF_SIZE, 0);

            if (n <= 0 || strcmp(buffer, THE_END) == 0) {
                break;
            }

            printf("Получено сообщение: %s\n", buffer);
        }
    }

    close(sock);

    return 0;
}
