#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define MESSAGE_LIMIT 10

int main() {
    int pipefd[2]; // Массив для файловых дескрипторов канала
    sem_t *sem; // Семафор для синхронизации обмена сообщениями

    // Создание неименованного канала
    if (pipe(pipefd) == -1) {
        perror("Ошибка при создании канала");
        exit(EXIT_FAILURE);
    }

    sem_unlink("/my_semaphore"); // Удаление семафора, если он уже существует

    // Создание семафора
    sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("Ошибка при создании семафора");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // Создание дочернего процесса

    if (pid == -1) {
        perror("Ошибка при создании дочернего процесса");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Код дочернего процесса
        close(pipefd[1]); // Закрытие дескриптора записи в канал

        for (int i = 0; i < MESSAGE_LIMIT; i++) {
            char message[256];

            if (i % 2 == 1) {
                sem_wait(sem); // Ожидание сигнала от родительского процесса
                read(pipefd[0], message, sizeof(message));
                printf("Дочерний процесс получил: %s\n", message);
            }

            snprintf(message, sizeof(message), "Message %d from child process", i + 1);
            printf("Дочерний процесс отправил: %s\n", message);
            write(pipefd[1], message, sizeof(message));
            sem_post(sem); // Отправка сигнала родительскому процессу
        }

        close(pipefd[0]); // Закрытие дескриптора чтения из канала
        sem_close(sem); // Закрытие семафора
        exit(EXIT_SUCCESS);
    } else {
        // Код родительского процесса
        close(pipefd[0]); // Закрытие дескриптора чтения из канала

        for (int i = 0; i < MESSAGE_LIMIT; i++) {
            char message[256];

            if (i % 2 == 0) {
                snprintf(message, sizeof(message), "Message %d from parent process", i + 1);
                printf("Родительский процесс отправил: %s\n", message);
                write(pipefd[1], message, sizeof(message));
            }

            sem_wait(sem); // Ожидание сигнала от дочернего процесса
            read(pipefd[0], message, sizeof(message));
            printf("Родительский процесс получил: %s\n", message);
            sem_post(sem); // Отправка сигнала дочернему процессу
        }

        close(pipefd[1]); // Закрытие дескриптора записи в канал
        sem_close(sem); // Закрытие семафора
        wait(NULL); // Ожидание завершения дочернего процесса
        exit(EXIT_SUCCESS);
    }
}

