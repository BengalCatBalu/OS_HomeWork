
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHM_KEY 12345
#define NUMBER_RANGE 100

typedef struct {
    int number;
    int finished;
} SharedData;

int main() {
    int shmid;
    SharedData *sharedData;
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Получение доступа к сегменту разделяемой памяти
    shmid = shmget(SHM_KEY, sizeof(SharedData), 0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Прикрепление сегмента к адресному пространству клиента
    sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if (sharedData == (SharedData *)-1) {
        perror("shmat");
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

    // Отключение сегмента разделяемой памяти
    shmdt(sharedData);

    printf("Завершение работы клиента.\n");

    return 0;
}