#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#define N 5
#define M 5

int array[N][M];

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

int main(int argc, char *argv[])
{
  int estado = 0;
  int c, f, i, dato;
  //Mostrar el tiempo
  struct timeval t_ini, t_fin;
  double secs;
  //Partir la matriz
  int v = atoi(argv[1]);
  int trozo[v+1];
  double aux = (double) N/v;
  //Tuberias
  int t[v][2];
  for (int i = 0; i < v; i++) //inicialización de las tuberías
    pipe(t[i]);

  //Repartir las columnas de la matriz
  for(i=0; i < (v+1); i++)
    trozo[i] = (int) (aux * i);
  //Semilla para el random
  srand ( time(NULL) );
  //Rellenar matriz
  for ( c = 0 ; c < N ; c++)
    for ( f = 0 ; f < M ; f++ )
      array[c][f] = rand()%10;

  for(c = 0; c < N; c++)
  {
    for(f = 0; f < M; f++)
      printf("%d\t", array[c][f]);

    printf("\n\n");
  }

  gettimeofday(&t_ini, NULL); /* obtiene tiempo inicial */

  //Modo ProGrammer
  for(i = 0; i < v; i++)
  {
    if(fork() == 0)
    {
      //Cerrar el canal de escritura de la tuberia correspondiente
      close(t[i][0]);

      //Ordenar matriz
      for(c = trozo[i]; c < trozo[i+1]; c++)
        ordena(c);

      //Introducir datos en la tubería
      for(c = trozo[i]; c < trozo[i+1]; c++)
      {
        for(f = 0; f < M; f++)
        {
          dato = array[c][f];
          write(t[i][1], &dato, sizeof(dato));
        }
      }

      close(t[i][1]); //Cerrar tubería
      exit(0);  //Cerrar proceso
    }
  }
  //Esperar a que todos los hijos acaben
  for(i = 0; i < v; i++)
    wait(&estado);

  gettimeofday(&t_fin, NULL); /* obtiene tiempo final */

  secs = timeval_diff(&t_fin, &t_ini); /* Obtiene el tiempo consumido */
  printf("Tiempo consumido: %.16g milisegundos\n\n", secs * 1000.0);

  //Cerrar los canales de escritura de la tubería
  for(int i = 0; i < v; i++)
    close(t[i][1]);

  //Leer los datos y modificar la matriz
  for(i = 0; i < v; i++)
  {
    for(c = trozo[i]; c < trozo[i+1]; c++)
      for(f = 0; f < M; f++)
      {
        read(t[i][0], &dato, sizeof(int));
        array[c][f] = dato;
      }

      close(t[i][0]); //Cerrar la tubería
  }

  //Imprimir matriz
  for(c = 0; c < N; c++)
  {
    for(f = 0; f < M; f++)
      printf("%d\t", array[c][f]);

    printf("\n\n");
  }

  exit(0);
}
