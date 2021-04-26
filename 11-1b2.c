#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="11-1a2.c";
  key_t  key;
  int len, maxlen;

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

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  printf("Program 2 started.\n");

  for (size_t i = 1; i <= 5; i++)
  {
    len = sizeof(mybuf.mes);
    if (msgrcv(msqid, (struct msgbuf *) &mybuf, len, 1, 0) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("2 Program recieved message: message type = %ld, fMessage = %i, sMessage = %i\n", mybuf.mtype, mybuf.mes.messageF, mybuf.mes.messageS);
  }

  printf("2 Program send 6 messages to 1 program.\n");
  for (int i = 1; i <= 6; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 2;
    mybuf.mes.messageF = 1945;
    mybuf.mes.messageS = 1941;
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

  printf("2 Program sent messages.\n");
  
  return 0;
}
