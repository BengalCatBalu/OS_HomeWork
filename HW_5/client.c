#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256
#define FIFO_NAME "myfifo"

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // Открытие именованного канала для записи
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Ошибка при открытии канала");
        return 1;
    }

    while (1) {
        // Чтение сообщения от пользователя
        printf("Введите сообщение (exit для выхода): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Запись сообщения в именованный канал
        ssize_t bytesWritten = write(fd, buffer, strlen(buffer));
        if (bytesWritten <= 0) {
            perror("Ошибка при записи в канал");
            break;
        }

        // Проверка на завершение соединения
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Соединение завершено. Завершение программы.\n");
            break;
        }
    }

    // Закрытие именованного канала
    close(fd);

    return 0;
}
