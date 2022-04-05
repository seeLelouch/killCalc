#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Not enough arguments\n");
        system("PAUSE");
        return 0;
    }

    printf("Reached end of program\n");
    system("PAUSE");
    return 0;
}