#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c
int main(void)
{
    int    msqid;      // IPC descriptor for the message queue
    char   pathname[] = "2a.c"; // The file name used to generate the key.
                                // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int    len, maxlen;       // Cycle counter and the length of the informative part of the message
    
    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        
        struct 
        {
            short S;
            float F;

        } details;

    } mybuf;
    
    
    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    for (int i = 0; i < 3; i++) {
    
        mybuf.mtype = 5;
        mybuf.details.F = 22.22;

        len = sizeof(mybuf.details);
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }
    
    maxlen = sizeof(mybuf.details);

    for (int i = 0; i < 3; i++) {

        if ((len = msgrcv(msqid, (struct mymsgbuf*)&mybuf, maxlen, 1, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        printf("2b.c: received an int: %d\n", mybuf.details.S);
    }

    return 0;
}