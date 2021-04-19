#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

//Initially semaphore is 0
//Functions: increase - increase value by 1, decrease - decrease value by 1
//If value < 0 - wait

int increase(int semid, struct sembuf* buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int decrease(int semid, struct sembuf* buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int main()
{
    int parent[2], result, semid, N, k = 0;
    char pathname[] = "05-3.c", resstring[14];
    key_t key;
    size_t size = 0;
    struct sembuf buffer;
    //  pipe
    if (pipe(parent) < 0) {
        printf("Can't open parent's pipe\n\r");
        exit(-1);
    }
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Key not generated\n");
        exit(-1);
    }
    // semaphore
    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not find \n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can't get semid\n");
            exit(-1);
        }
        printf("Semaphore created\n");
    }
    result = fork();

    if (result < 0) {
        printf("Can't fork\n\r");
        exit(-1);
    }
        //Parent logic
    else if (result > 0) {
        printf("Enter N: ");
        scanf("%d", &N);

        if (N < 2) {
            printf("N >= 2 only\n");
            exit(-1);
        }

        for (size_t i = 1; i <= N; i++)
        {
            if (write(parent[1], "Hello everyone!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }
            printf("Pair №%ld, parent sent message\n\r", i);
            increase(semid, &buffer);
            decrease(semid, &buffer);
            size = read(parent[0], resstring, 14);
            if (size != 14) {
                printf("Can\'t read string from child\n\r");
                exit(-1);
            }
            printf("Message for parent came. %s\n\r", resstring);
        }
        close(parent[0]);
    }
        //Child :
    else {
        while(1) {
            //Wait parent's text
            decrease(semid, &buffer);
            size = read(parent[0], resstring, 14);
            if (size < 0) {
                close(parent[1]);
                close(parent[0]);
                return 0;
            }
            k++;
            //Parent's text
            printf("Pair №%d, child get message. %s\n\r", k, resstring);
            //Send message back
            if (write(parent[1], "Privet!", 14) != 14) {
                printf("cant write  line\n");
                exit(-1);
            }
            //Continue
            increase(semid, &buffer);
        }
    }
    return 0;
}