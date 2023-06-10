#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <порт1> <порт2> ... <портN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in receiver_addr;
    char buffer[MAX_BUFFER_SIZE];
    int num_receivers = argc - 1;

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка параметров отправителя
    struct sockaddr_in sender_addr;
    memset(&sender_addr, 0, sizeof(sender_addr));
    sender_addr.sin_family = AF_INET;
    sender_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sender_addr.sin_port = htons(0);  // Используем случайный порт

    // Привязка сокета отправителя к адресу и порту
    if (bind(sockfd, (struct sockaddr *)&sender_addr, sizeof(sender_addr)) < 0) {
        perror("Ошибка привязки сокета");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Введите сообщение: ");
        fgets(buffer, sizeof(buffer), stdin);

        for (int i = 1; i <= num_receivers; ++i) {
            // Настройка параметров получателя
            memset(&receiver_addr, 0, sizeof(receiver_addr));
            receiver_addr.sin_family = AF_INET;
            receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            receiver_addr.sin_port = htons(atoi(argv[i]));

            // Отправка сообщения
            ssize_t num_bytes = sendto(sockfd, buffer, strlen(buffer), 0,
                                       (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
            if (num_bytes < 0) {
                perror("Ошибка отправки сообщения");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Закрытие сокета
    close(sockfd);

    return 0;
}










