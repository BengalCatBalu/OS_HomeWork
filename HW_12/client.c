#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h> // Добавлен заголовочный файл unistd.h


#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE];

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Задание параметров адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_aton(SERVER_IP, &server_address.sin_addr) == 0) {
        perror("Ошибка адреса сервера");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Введите сообщение для отправки на сервер: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        // Отправка сообщения на сервер
        if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_address,
                   sizeof(server_address)) < 0) {
            perror("Ошибка отправки сообщения");
            exit(EXIT_FAILURE);
        }

        // Прием ответа от сервера
        socklen_t server_address_length = sizeof(server_address);
        int message_length = recvfrom(client_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&server_address,
                                      &server_address_length);
        if (message_length < 0) {
            perror("Ошибка приема сообщения");
            exit(EXIT_FAILURE);
        }

        // Вывод полученного сообщения
        printf("Получен ответ от сервера: %s", buffer);
    }

    // Закрытие сокета клиента
    close(client_socket);

    return 0;
}

