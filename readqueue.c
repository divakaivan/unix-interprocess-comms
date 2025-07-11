#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int main(void) {
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    // same as the producer
    if ((key = ftok("send2queue.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    // connect to the queue
    if ((msqid = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }
    printf("receiver: ready to receive messages.\n");
    for (;;) {
        if (msgrcv(msqid, &buf, sizeof buf.mtext, 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("receiver: \"%s\"\n", buf.mtext);
    }
    return 0;
}
