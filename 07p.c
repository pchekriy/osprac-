#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "stdlib.h"
int main()
{
    int shmid;
    key_t key;
    char pathname[] = "07p.c";
    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    int filesize = 0;
    // из примера
    if ((shmid = shmget(key, sizeof(int) + filesize * sizeof(char), 0666|IPC_CREAT)) < 0) {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
    int *file_size_ptr;
    if ((file_size_ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    filesize = *file_size_ptr;
    char *filetext;
    filetext = (char*)(file_size_ptr + 1);
    for (int i = 0; i < filesize; i++){
        putchar(filetext[i]);
    }
    if (shmdt(file_size_ptr) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can't delete shared memory\n");
        exit(-1);
    }
    return 0;
}