#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

#define SHM_NAME "/my_shared_memory"
#define NUMBER_RANGE 100

typedef struct {
    int number;
    int finished;
} SharedData;

int main() {
    int shm_fd;
    SharedData *sharedData;
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Получение доступа к разделяемой памяти
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Отображение разделяемой памяти на адресное пространство клиента
    sharedData = (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedData == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Генерация и отправка случайных чисел...\n");

    // Цикл генерации случайных чисел и отправки их серверу
    while (!sharedData->finished) {
        if (sharedData->number == -1) {
            int randomNum = rand() % NUMBER_RANGE;
            sharedData->number = randomNum;
            printf("Отправлено число: %d\n", randomNum);
        }

        usleep(100000); // Задержка для снижения нагрузки
    }

    // Отключение от разделяемой памяти
    if (munmap(sharedData, sizeof(SharedData)) == -1) {
        perror("munmap");
        exit(1);
    }

    printf("Завершение работы клиента.\n");

    return 0;
}
