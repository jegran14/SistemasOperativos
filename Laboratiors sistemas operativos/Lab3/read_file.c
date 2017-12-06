#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main (int argc, char * argv[])
{
    int fd;
    char dato;
    
    fd = open(argv[1], O_RDONLY);
    
    if(fd == -1) printf( "Ha ocurrido un error");
    
    else
    {
        while (read(fd, &dato, sizeof(dato)) > 0)
            printf("%c", dato);
    }
    
    printf("\n");
    
    close(fd);
    exit(0);
}
