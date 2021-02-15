#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
    int     fd;
    size_t  size;
    char    string[] = "Hello, world!";
    char    resultstring[14];
    (void)umask(0);
    if ((fd = open("myfile", O_RDONLY | O_CREAT , 0666)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    size = read(fd, resultstring, 14);
    printf("%s\n", resultstring);
    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }
    return 0;
}
