#include <stdio.h>
#include "_stdlib.h"
int main(int ac, char **av)
{
    char stream[100] = "";
    _strncat(stream, av[1], _strlen(av[1]));
    
    _strncat(stream, av[2], _strlen(av[2]));
    printf("printf stream %s\n",stream);
    return(0);
    
}