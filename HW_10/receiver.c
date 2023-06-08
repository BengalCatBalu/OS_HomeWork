#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sendpid;
int number = 0;

int step = 0;
void getFirst() {
  ++step;

  if (step == 32) {
    printf("Результат: %d \n", number);
    exit(0);
  } else {
    printf("Получен %d бит: %d \n", step, 0);
    kill(sendpid, SIGUSR1);
  }
}

void getSecond() {
  number += (1 << step);
  ++step;

  if (step == 32) {
    printf("Результат: %d \n", number);
    exit(0);
  } else {
    printf("Получен %d бит: %d \n", step, 1);
    kill(sendpid, SIGUSR1);
  }
}

int main(void) {
  (void)signal(SIGUSR1, getFirst);
  (void)signal(SIGUSR2, getSecond);

  printf("Receiver PID: %d \n", getpid());

  printf("Sender PID: ");
  scanf("%d", &sendpid);

  while(1) {};
}



