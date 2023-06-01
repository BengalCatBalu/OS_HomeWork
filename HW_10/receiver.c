#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int received_number = 0; // Принятое число
int bit_index = 0; // Индекс текущего бита для приема

// Обработчик сигналов SIGUSR1 и SIGUSR2
void sigusr_handler(int signum) {
    if (signum == SIGUSR1) {
        // Принят бит 0
        received_number |= (0 << bit_index);
    } else if (signum == SIGUSR2) {
        // Принят бит 1
        received_number |= (1 << bit_index);
    }

    // Подтверждение принятия бита с помощью SIGUSR1
    kill(getpid(), SIGUSR1);

    bit_index++;

    if (bit_index == 32) {
        // Печать принятого числа
        printf("Принятое число: %d\n", received_number);

        // Завершение работы
        exit(0);
    }
}

int main() {
    printf("Программа-приемник (PID: %d)\n", getpid());

    // Установка обработчика сигналов SIGUSR1 и SIGUSR2
    signal(SIGUSR1, sigusr_handler);
    signal(SIGUSR2, sigusr_handler);

    // Ожидание сигналов
    while (1) {
        sleep(1);
    }

    return 0;
}
