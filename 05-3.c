#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    int pfd;
    int fd[2];
    char a[100];
    int size=0;
    int c,b;
    if((pfd=pipe(fd))<0)
    {
        exit(1);
    }
    else
    {
        do
        {
            c=write(fd[1],"h",1);
            size=(size+1);//так как записываем 2 байта
            printf("%d\n",size);
        }
        while(c != b);
    }
    return 0;
}