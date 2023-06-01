#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pid_t receiver_pid; // PID программы-приемника
int bit_index = 0; // Индекс текущего бита для передачи

// Обработчик сигнала SIGUSR1
void sigusr1_handler(int signum) {
    // Передача следующего бита
    if (bit_index >= 0) {
        int bit = (receiver_pid >> bit_index) & 1;
        if (bit == 0) {
            // Передача бита 0 с помощью SIGUSR1
            kill(receiver_pid, SIGUSR1);
        } else {
            // Передача бита 1 с помощью SIGUSR2
            kill(receiver_pid, SIGUSR2);
        }
        bit_index--;
    } else {
        // Завершение передачи
        kill(receiver_pid, SIGINT);
    }
}

int main() {
    printf("Программа-передатчик (PID: %d)\n", getpid());

    // Ввод PID программы-приемника
    printf("Введите PID программы-приемника: ");
    scanf("%d", &receiver_pid);

    // Установка обработчика сигнала SIGUSR1
    signal(SIGUSR1, sigusr1_handler);

    // Ожидание сигнала SIGUSR1
    while (1) {
        sleep(1);
    }

    return 0;
}
