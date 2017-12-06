#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {

//  if (fork()!=0) /* I am the parent */
//    printf("Myselself: %d. My parent: %d\n", getpid(), getppid());
//  else      /* I am the child */
//    if (fork()!=0) /* I am the child, now parent */
//      printf("Myself: %d. My parent: %d\n", getpid(), getppid());
//    else      /* I am the child of the child */
//      printf("Myself: %d. My parent: %d\n", getpid(), getppid());

int n = atoi(argv[argc - 1]);
int i;

  for(i = 0; i < n; i++)
  {
	if(fork()!=0) /* I am the father */
	{
		printf("Myself: %d. My parent: %d siendo padre\n", getpid(), getppid());
		wait(0);
		break;
	}
	
	else      /* I am the child */
	     printf("Myself: %d. My parent: %d siendo hijo\n", getpid(), getppid());
  }
  exit(0);
}
