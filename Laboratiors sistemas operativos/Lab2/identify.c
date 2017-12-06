#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  pid_t myself, father;

  myself = getpid();
  father = getppid();

  printf("I am the process with ID: %d.\n", myself);
  printf("My parent process has ID: %d \n", father);
  exit(0);
}
