#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int t1[2], t2[2], t3[2];
  int testigo, i = 0, siguiente = 1;
  int n = atoi(argv[1]);
  //abrir tuberias
  pipe(t1);
  pipe(t2);
  pipe(t3);

  write(t3[1], &siguiente, sizeof(int));
  //p1
  if(fork() == 0)
  {
    //Cerrar las tuberias que nos interesan
    close(t1[0]);//Escribe
    close(t2[0]);//No usa
    close(t2[1]);//No usa
    close(t3[1]); //Lee

    int siguiente = 2;

    while(i < n)
    {
      read(t3[0], &testigo, sizeof(int));
      if(testigo == 1)
      {
        printf("P1: %d - %d\n", getpid(), i);
        i++;
        write(t1[1], &siguiente, sizeof(int));
      }
    }

    exit(0);
  }

  //p2
  if(fork() == 0)
  {
    close(t1[1]); //Lee
    close(t2[0]); //Escribe
    close(t3[0]); //No usa
    close(t3[1]); //No usa

    int siguiente = 3;

    while( i < n)
    {
      read(t1[0], &testigo, sizeof(int));
      if(testigo == 2)
      {
        printf("P2: %d - %d\n", getpid(), i);
        i++;
        write(t2[1], &siguiente, sizeof(int));
      }
    }

    exit(0);
  }

  //p3
  if(fork() == 0)
  {
    close(t2[1]); //Lee
    close(t3[0]); //Escribe
    close(t1[0]); //No usa
    close(t1[1]); //No usa

    int siguiente = 1;

    while(i < n)
    {
      read(t2[0], &testigo, sizeof(int));
      if(testigo == 3)
      {
        printf("P3: %d - %d\n", getpid(), i);
        i++;
        write(t3[1], &siguiente, sizeof(int));
      }
    }
    exit(0);
  }

  exit(0);
}
