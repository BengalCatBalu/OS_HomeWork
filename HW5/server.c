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

    // Создание именованного канала
    mkfifo(FIFO_NAME, 0666);

    printf("Ожидание подключения клиента...\n");

    // Открытие именованного канала для чтения
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка при открытии канала");
        return 1;
    }

    printf("Клиент подключен. Ожидание сообщений...\n");

    while (1) {
        // Чтение сообщения от клиента
        ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
        if (bytesRead <= 0) {
            perror("Ошибка при чтении из канала");
            break;
        }

        // Проверка на завершение соединения
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Клиент отключен. Завершение программы.\n");
            break;
        }

        printf("Получено сообщение от клиента: %s", buffer);
    }

    // Закрытие и удаление именованного канала
    close(fd);
    unlink(FIFO_NAME);

    return 0;
}
