#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Функция для вычисления факториала
int factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

// Функция для вычисления числа Фибоначчи
int fibonacci(int n) {
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./program <number>\n");
        return 1;
    }

    int num = atoi(argv[1]);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Процесс-ребенок
        int fact = factorial(num);
        printf("Факториал числа %d равен %d\n", num, fact);
    } else {
        // Родительский процесс
        wait(NULL);
        int fib = fibonacci(num);
        printf("Число Фибоначчи с индексом %d равно %d\n", num, fib);
    }

    return 0;
}
