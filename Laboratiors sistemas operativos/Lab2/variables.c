#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  int x=0;

  if (fork()!=0) x=11;
  else
    if (fork()!=0) x=5;
    else x=81;
  printf("Mi valor de x es: %d\n", x);
  exit(0);
}
