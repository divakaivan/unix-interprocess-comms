#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    int pfds[2];
    char buf[30];
    pipe(pfds);
    if (!fork()) {
        printf("    child: writing to pipe\n");
        write(pfds[1], "test", 5);
        printf("    child exiting\n");
        exit(0);
    } else {
        printf("parent reading from pipe\n");
        read(pfds[0], buf, 5);
        printf("parent read \"%s\"\n", buf);
        wait(NULL); // wait for any child to finish
    }
    return 0;
}
