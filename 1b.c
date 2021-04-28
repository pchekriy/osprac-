#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c
int main(void) {
    int msqid;      // IPC descriptor for the message queue
    char pathname[] = "1a.c"; // The file name used to generate the key.
    // A file with this name must exist in the current directory.
    key_t key;     // IPC key
    int i, len;       // Cycle counter and the length of the informative part of the message

    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        struct {
            short S;
            float F;

        } details;
    } mybuf;


    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    for (;;) {
        len = sizeof(mybuf.details);

        if ((len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, 0, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(0);
        }
        if (mybuf.mtype == 1) {
            printf("Float received: %f \n", mybuf.details.F);
        } else if (mybuf.mtype == 2) {
            printf("integer received: %d \n", mybuf.details.S);
        } else {
            printf("Error, unexpected type. \n");
        }
    }
    return 0;
}
}
