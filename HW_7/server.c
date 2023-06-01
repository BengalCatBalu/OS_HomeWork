#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME "/my_shared_memory"

typedef struct {
    int number;
    int finished;
} SharedData;

int main() {
    int shm_fd;
    SharedData *sharedData;

    // Создание разделяемой памяти
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Определение размера разделяемой памяти
    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Отображение разделяемой памяти на адресное пространство сервера
    sharedData = (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedData == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Ожидание данных от клиента...\n");

    // Цикл чтения данных от клиента
    while (!sharedData->finished) {
        // Проверка наличия новых данных
        if (sharedData->number != -1) {
            printf("Получено число от клиента: %d\n", sharedData->number);
            sharedData->number = -1; // Сброс значения числа
        }

        usleep(100000); // Задержка для снижения нагрузки
    }

    // Отключение от разделяемой памяти
    if (munmap(sharedData, sizeof(SharedData)) == -1) {
        perror("munmap");
        exit(1);
    }

    // Закрытие разделяемой памяти
    if (close(shm_fd) == -1) {
        perror("close");
        exit(1);
    }

    // Удаление разделяемой памяти
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    printf("Завершение работы сервера.\n");

    return 0;
}

