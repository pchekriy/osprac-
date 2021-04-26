#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="11-1a2.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    int mtype;
    struct 
    {
      int messageF;
      int messageS;
    } mes;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("Program 1 started and send 5 messages to 2 program.\n");
  for (i = 1; i <= 5; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.mes.messageF = 555;
    mybuf.mes.messageS = 2001;
    len = sizeof(mybuf.mes);
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  printf("1 Program sent messages.\n");


  for (size_t i = 1; i <= 6; i++)
  {
    len = sizeof(mybuf.mes);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 2, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("1 Program recieved message: message type = %ld, fMessage = %i, sMessage = %i\n", mybuf.mtype, mybuf.mes.messageF, mybuf.mes.messageS);
  }

  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

  return 0;
}
