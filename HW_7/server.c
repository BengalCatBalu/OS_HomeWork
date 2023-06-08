#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    // Генерируем ключ для разделяемой памяти
    key_t key = ftok("shared_memory_key", 1234);

    // Создаем сегмент разделяемой памяти
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);

    // Присоединяем сегмент к адресному пространству процесса
    char *shm = (char *)shmat(shmid, NULL, 0);

    // Инициализация переменных
    int count = 0;
    int total_numbers = 30;

    // Цикл чтения и вывода чисел из разделяемой памяти
    while (count < total_numbers) {
        // Ожидание появления нового числа в разделяемой памяти
        while (shm[0] == '\0') {
            sleep(1);
        }

        // Чтение числа из разделяемой памяти
        int number = atoi(shm);

        // Вывод числа на экран
        printf("Received number: %d\n", number);

        // Обнуление разделяемой памяти
        shm[0] = '\0';

        count++;
    }

    // Отсоединение сегмента разделяемой памяти
    shmdt(shm);

    // Удаление сегмента разделяемой памяти
    shmctl(shmid, IPC_RMID, NULL);

    printf("Server terminated\n");

    return 0;
}

