#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "fifo_chapter"

int main(void) {
    char s[300];
    int num, fd;

    mkfifo(FIFO_NAME, 0644);
    printf("waiting for readers...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    printf("got a reader ~ type stuff to send\n");

    while (fgets(s, sizeof(s), stdin), !feof(stdin)) {
        if ((num = write(fd, s, strlen(s))) == -1)
            perror("write");
        else
            printf("fifo_speak: wrote %d bytes\n", num);
    }
    return 0;
}
