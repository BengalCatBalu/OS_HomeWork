#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[MAX_BUFFER_SIZE];

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Задание параметров адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу сервера
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Ошибка привязки сокета");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание сообщений...\n");

    while (1) {
        socklen_t client_address_length = sizeof(client_address);

        // Прием сообщения от клиента
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int message_length = recvfrom(server_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_address,
                                      &client_address_length);
        if (message_length < 0) {
            perror("Ошибка приема сообщения");
            exit(EXIT_FAILURE);
        }

        // Вывод принятого сообщения
        printf("Получено сообщение от клиента: %s", buffer);

        // Отправка сообщения всем клиентам
        if (sendto(server_socket, buffer, message_length, 0, (struct sockaddr *)&client_address,
                   client_address_length) < 0) {
            perror("Ошибка отправки сообщения");
            exit(EXIT_FAILURE);
        }
    }

    // Закрытие сокета сервера
    close(server_socket);

    return 0;
}
