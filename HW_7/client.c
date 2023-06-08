#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHM_SIZE 1024

int main() {
    // Генерируем ключ для разделяемой памяти
    key_t key = ftok("shared_memory_key", 1234);

    // Получаем идентификатор сегмента разделяемой памяти
    int shmid = shmget(key, SHM_SIZE, 0666);

    // Присоединяем сегмент к адресному пространству процесса
    char *shm = (char *)shmat(shmid, NULL, 0);

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Генерация и запись случайных чисел в разделяемую память
    int count = 0;
    int total_numbers = 30;

    while (count < total_numbers) {
        // Генерация случайного числа
        int number = rand() % 100;

        // Запись числа в разделяемую память
        sprintf(shm, "%d", number);

        // Вывод информации об отправленном числе
        printf("Send number: %d\n", number);
        // Задержка в 3 секунды
        sleep(3);

        count++;
    }

    // Отсоединение сегмента разделяемой памяти
    shmdt(shm);

    printf("Client terminated\n");

    return 0;
}