#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    if (argc < 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(1);
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server at %s:%d\n", server_ip, server_port);

    // Прием сообщений от сервера
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Чтение сообщения от сервера
        ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        printf("Client 2: %s", buffer);

        // Проверка условия завершения работы
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }
    }

    // Закрытие сокета
    close(client_socket);

    printf("Client closed\n");

    return 0;
}


