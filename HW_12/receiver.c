#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <порт>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in receiver_addr;
    char buffer[MAX_BUFFER_SIZE];
    int port = atoi(argv[1]);

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка параметров получателя
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    receiver_addr.sin_port = htons(port);

    // Привязка сокета получателя к адресу и порту
    if (bind(sockfd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("Ошибка привязки сокета");
        exit(EXIT_FAILURE);
    }

    printf("Программа получателя привязана к порту: %d\n", port);
    printf("Ждем сообщения...\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Получение сообщения
        ssize_t num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (num_bytes < 0) {
            perror("Ошибка приема сообщения");
            exit(EXIT_FAILURE);
        }

        // Вывод полученного сообщения
        printf("Получено сообщение: %s", buffer);
    }

    // Закрытие сокета
    close(sockfd);

    return 0;
}











