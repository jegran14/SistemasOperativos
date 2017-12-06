#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>

#define N 10
#define M 10

long int array[N][M];

/* devuelve "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b) {
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

void ordena(int numero_aux)
{
  int position, d, c;
  long int swap;

  for ( c = 0 ; c < ( M - 1 ) ; c++ )
    {
      position = c;

      for ( d = c + 1 ; d < M ; d++ )
	{
	  if ( array[numero_aux][position] > array[numero_aux][d] )
            position = d;
	}
      if ( position != c )
	{
	  swap = array[numero_aux][c];
	  array[numero_aux][c] = array[numero_aux][position];
	  array[numero_aux][position] = swap;
	}
    }

}

int main()
{
  int estado = 0;
  int c1, c2, valor;

  //Scheduler
  int policy, ret, x;
  struct sched_param sp;

  struct timeval t_ini, t_fin;
  double secs;


  srand ( time(NULL) );

  for ( c1 = 0 ; c1 < N ; c1++)
    for ( c2 = 0 ; c2 < M ; c2++ )
      array[c1][c2] = rand();

  gettimeofday(&t_ini, NULL); /* obtiene tiempo inicial */

  //Modo ProGrammer
  if(fork() == 0)
  {
    sched_getparam(0 ,&sp);
    sp.sched_priority = 1;
    sched_setscheduler(0, SCHED_RR, &sp);
    sp.sched_priority = 11;
    sched_setparam(0, &sp);

    gettimeofday(&t_ini, NULL); /* obtiene tiempo inicial */

    for(c1 = 0; c1 < N/2; c1++)
        ordena(c1);

    gettimeofday(&t_fin, NULL); /* obtiene tiempo final */

    secs = timeval_diff(&t_fin, &t_ini); /* Obtiene el tiempo consumido */
    printf("Tiempo consumido primer hijo: %.16g milisegundos\n\n", secs * 1000.0);

    exit(0);
  }
  if(fork() == 0)
  {
      gettimeofday(&t_ini, NULL); /* obtiene tiempo inicial */

      for(c1 = N/2; c1 < N; c1++)
          ordena(c1);

      gettimeofday(&t_fin, NULL); /* obtiene tiempo final */
      secs = timeval_diff(&t_fin, &t_ini); /* Obtiene el tiempo consumido */
      printf("Tiempo consumido segundo hijo: %.16g milisegundos\n\n", secs * 1000.0);

      exit(0);
  }
  wait(& estado);
  wait(& estado);


  gettimeofday(&t_fin, NULL); /* obtiene tiempo final */

  secs = timeval_diff(&t_fin, &t_ini); /* Obtiene el tiempo consumido */
  printf("Tiempo consumido: %.16g milisegundos\n\n", secs * 1000.0);

  /*printf("Listas ordenadas en orden ascendente:\n");
  for ( c1 = 0 ; c1 < N ; c1++)
    {
      printf("Columna %d: ", c1);
      for ( c2 = 0 ; c2 < M ; c2++ )
	printf("%ld ", array[c1][c2]);
      printf("\n");
    }*/

  exit(0);
}
