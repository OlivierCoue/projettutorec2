#include <stdio.h>
#include <stdlib.h>


int main(const int _argc, const char * _argv[])
{
    int i;
    for(i=1; i<_argc; i++){
        printf("<%s>", _argv[i] );
    }
    return 0;
}