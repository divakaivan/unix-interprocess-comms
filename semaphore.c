#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX_RETRIES 10

#ifdef NEED_SEMUN
union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};
#endif

int initsem(key_t key, int nsems) {
    int i;
    union semun arg;
    struct semid_ds buf;
    struct sembuf sb;
    int semid;

    semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

    if (semid >= 0) {
        // got it first
        sb.sem_op = 1; // POS->release resources
        sb.sem_flg = 0;
        arg.val = 1;
        printf("press RETURN\n");
        getchar();

        for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) {
            // do a semop to free the semaphores
            // set the sem_otime as needed below
            if (semop(semid, &sb, 1) == -1) {
                int e = errno;
                semctl(semid, 0, IPC_RMID); // clean up
                // IPC_RMID -> remove the sem from the system
                errno = e;
                return -1;
            }
        }
    } else if (errno == EEXIST) {
        // someone else already got it first
        int ready = 0;
        semid = semget(key, nsems, 0);
        if (semid < 0) return semid;

        arg.buf = &buf;
        for (i = 0; i < MAX_RETRIES && !ready; i++) {
            semctl(semid, nsems-1, IPC_STAT, arg);
            if (arg.buf->sem_otime != 0) {
                ready = 1;
            } else {
                sleep(1);
            }
        }
        if (!ready) {
            errno = ETIME;
            return -1;
        }
    } else {
        return semid;
    }
    return semid;
}

int main(void) {
    key_t key;
    int semid;
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;

    if ((key = ftok("semaphore.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    // get the sem created by seminit
    if ((semid = initsem(key, 1)) == -1) {
        perror("initsem");
        exit(1);
    }

    printf("press RETURN to lock: ");
    getchar();
    printf("trying to lock ~ \n");

    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }

    printf("locked!\n");
    printf("press RETURN to unlock: ");
    getchar();

    sb.sem_op = 1; // free resource
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
    printf("unlocked!");
    return 0;
}

