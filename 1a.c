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
    char   pathname[] = "1a.c"; // The file name used to generate the key.
                                // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int    i, len;       // Cycle counter and the length of the informative part of the message
    
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
    
    for (i = 0; i <= 5; i++) {
        if (i % 2 == 0) {
            mybuf.details.S = 12;
            mybuf.mtype = 2;
        } else {
            mybuf.details.F = 2.34;
            mybuf.mtype = 1;
        }
        len = sizeof(mybuf.details);
    
        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }
    
    mybuf.mtype = LAST_MESSAGE;
    len         = 0;
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    return 0;
}
