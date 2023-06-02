#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

    // Прослушивание входящих подключений
    if (listen(server_socket, 1) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d\n", port);

    // Ожидание подключения клиента
    socklen_t client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1) {
        perror("accept");
        exit(1);
    }

    printf("Client connected\n");

    // Перенаправление сообщений от клиента №1 клиенту №2
    while (1) {
        // Чтение сообщения от клиента №1
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        printf("Client 1: %s", buffer);

        // Проверка условия завершения работы
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }

        // Отправка сообщения клиенту №2
        if (write(client_socket, buffer, strlen(buffer)) == -1) {
            perror("write");
            exit(1);
        }
    }

    // Закрытие сокетов
    close(client_socket);
    close(server_socket);

    printf("Server closed\n");

    return 0;
}


