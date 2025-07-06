#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
   struct flock fl = {
        .l_type = F_WRLCK, // write lock
        .l_whence = SEEK_SET, // l_start starts from beginning of file
        .l_start = 0, // starting offset in bytes of lock
        .l_len = 0, // len of lock region in bytes, 0 is till eof
        // .l_pid
    };
    int fd;
    if (argc > 1) {
        fl.l_type = F_RDLCK;
        printf("read lock obtained\n");
    } else {
        printf("write lock obtained\n");
    }

    if ((fd = open("file_locking.c", O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }
    printf("press <RETURN> to try to get lock: ");
    getchar();
    printf("trying to get lock...");

    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }

    printf("got lock\n");
    printf("press <RETURN> to release lock: ");
    getchar();

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }
    printf("unlocked!");
    close(fd);
    return 0;
}
