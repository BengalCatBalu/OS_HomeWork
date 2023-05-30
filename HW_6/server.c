#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345

typedef struct {
    int number;
    int finished;
} SharedData;

int main() {
    int shmid;
    SharedData *sharedData;

    // Создание сегмента разделяемой памяти
    shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Прикрепление сегмента к адресному пространству сервера
    sharedData = (SharedData *)shmat(shmid, NULL, 0);
    if (sharedData == (SharedData *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Ожидание данных от клиента...\n");

    // Цикл чтения данных от клиента
    int k = 0;
    while (!sharedData->finished && k != 20) {
        // Проверка наличия новых данных
        if (sharedData->number != -1) {
            printf("Получено число от клиента: %d\n", sharedData->number);
            sharedData->number = -1; // Сброс значения числа
        }

        usleep(100000); // Задержка для снижения нагрузки
        k++;
    }

    // Отключение сегмента разделяемой памяти
    shmdt(sharedData);

    // Удаление сегмента разделяемой памяти
    shmctl(shmid, IPC_RMID, NULL);

    printf("Завершение работы сервера.\n");

    return 0;
}