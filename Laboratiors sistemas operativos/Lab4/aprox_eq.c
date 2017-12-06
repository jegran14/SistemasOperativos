/* aprox_eq N v ---> obtiene los números enteros, entre el 0 y el N, de manera que (en N) haya v regiones de tamaño similar */
/* el resultado se guarda en el vector trozo[] */
/* ejemplo: aprox_eq 8 2 devuelve 0 4 8 */
/* ejemplo: aprox_eq 8 3 devuelve 0 2 5 8 */

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/wait.h>

int GetCPUCount()
{
  int i, count = 0;
  cpu_set_t cs;
  sched_getaffinity(0, sizeof(cs), &cs);
  for(i = 0; i < sizeof(cs); i++)
    if(CPU_ISSET(i, &cs)) count++;

  return count;
}

int main (int argc, char * argv[])
{
  int i, t[2], suma, dato, estado=0;
  int v = GetCPUCount();
  int N = atoi(argv[1]); /* sólo lee correctamente hasta el tamaño máximo de int */
  int trozo[v+1];
  double aux = (double) N/v; /* casting de tipos */

  pipe(t);

  for (i=0; i<(v+1); i++)
    trozo[i] = (int) (aux * i); /* casting de tipos */

  for (i = 0; i < v; i++)
  {
    if(fork() == 0)
    {
        suma = 0;

        close(t[0]);
        int j;
        for(j = trozo[i+1]; j > trozo[i]; j--)
        {
            suma += j;
        }
        printf("La suma de %d es: %d \n", i, suma);
        write(t[1], &suma, sizeof(suma));
        close(t[1]);
        exit(0);
    }
  }


  for (i=0; i<v; i++) {
    wait(& estado);
  }

  close(t[1]);
  suma = 0;
  while(read(t[0], &dato, sizeof(int)) > 0)
      suma = suma + dato;
  close(t[0]);
  printf("La suma es: %d \n", suma);
}
