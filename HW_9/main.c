#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1
#define MESSAGE_COUNT 10

int main() {
    int pipefd[2]; // Дескрипторы для канала
    pid_t pid;
    sem_t *sem; // Семафор для синхронизации

    // Создание канала
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // Создание семафора
    sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Создание дочернего процесса
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс

        close(pipefd[PIPE_READ_END]); // Закрытие ненужного конца канала

        // Циклический обмен сообщениями
        for (int i = 0; i < MESSAGE_COUNT; i++) {
            // Ожидание разрешения на запись
            sem_wait(sem);

            // Запись сообщения в канал
            printf("Дочерний процесс отправляет сообщение: Message %d\n", i + 1);
            write(pipefd[PIPE_WRITE_END], "Message", 7);

            // Разрешение чтения родительскому процессу
            sem_post(sem);
        }

        close(pipefd[PIPE_WRITE_END]); // Закрытие конца канала для записи

        // Удаление семафора
        if (sem_unlink("/my_semaphore") == -1) {
            perror("sem_unlink");
            exit(1);
        }



        printf("Дочерний процесс завершился.\n");
    } else {
        // Родительский процесс

        close(pipefd[PIPE_WRITE_END]); // Закрытие ненужного конца канала

        // Циклический обмен сообщениями
        for (int i = 0; i < MESSAGE_COUNT; i++) {
            // Ожидание разрешения на чтение
            sem_wait(sem);

            char buffer[8];
            // Чтение сообщения из канала
            read(pipefd[PIPE_READ_END], buffer, 7);
            printf("Родительский процесс принял сообщение: %s %d\n", buffer, i + 1);

            // Разрешение записи дочернему процессу
            sem_post(sem);
        }

        close(pipefd[PIPE_READ_END]); // Закрытие конца канала для чтения

        // Ожидание завершения дочернего процесса
        wait(NULL);

        // Удаление семафора
        if (sem_unlink("/my_semaphore") == -1) {
            perror("sem_unlink");
            exit(1);
        }

        printf("Родительский процесс завершился.\n");
    }

    return 0;
}
