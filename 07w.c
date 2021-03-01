#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "stdlib.h"

int main()
{
    int shmid;
    key_t key;
    FILE *file;
    char pathname[] = "07p.c";
    file = fopen(pathname, "r");
    fseek(file, 0L, SEEK_END);
    int filesize = 0;
    filesize = ftell(file);
    rewind(file);
    //из примера
    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if ((shmid = shmget(key, sizeof(int) + filesize * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
    int *file_size_ptr;
    if ((file_size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    *file_size_ptr = filesize;
    char *filetext;
    filetext = (char*)(file_size_ptr + 1);
    for (int i = 0; i < filesize; i++){
        filetext[i] = fgetc(file);
    }
    fclose(file);
    if (shmdt(file_size_ptr) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}