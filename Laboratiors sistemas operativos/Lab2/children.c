#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {

int n = atoi(argv[argc - 1]);
int i, j;
int sonId[2];
int estado = 0;

  for(i = 0; i < 2; i++)
  {
        sonId[i] = fork();
        
	if(sonId[i] == 0) /* I am the father */
	{
            for(j = 0; j < 2; j++)
            {
                sonId[j] = fork();
                
                if(sonId[j] == 0)
                {
                    printf("Myself: %d. My parent: %d. siendo nieto\n", getpid(), getppid());
                    exit(0);
                }
            }
            
            for(j = 0; j < 2; j++) wait(&estado);
            
            printf("Myself: %d. My parent: %d. My son1: %d, My son2: %d. siendo hijo\n", getpid(), getppid(), sonId[0], sonId[1]);
            exit(0);
	}
  }
  for(i = 0; i < 2; i++) wait(&estado);
  
  printf("Myself: %d. My parent: %d. My son1: %d, My son2: %d. siendo padre\n", getpid(), getppid(), sonId[0], sonId[1]);
  exit(0);
}
