#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void) {
    key_t key;
    int semid;
    union semun arg;

    if ((key = ftok("semaphore.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    // get sem created by seminit
    if ((semid = semget(key, 1, 0)) == -1) {
        perror("semget");
        exit(1);
    }

    // remove it
    if (semctl(semid, 0, IPC_RMID, 0) == -1) {
        perror("semctl");
        exit(1);
    }
    return 0;
}
