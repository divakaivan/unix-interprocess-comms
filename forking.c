#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int rv;

    // signal(SIGCHLD, SIG_IGN);
    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(1);
    case 0: // child
        printf("this is a child process\n");
        printf("pid = %d\n", getpid());
        printf("pid's parent pid = %d\n", getppid());
        printf("enter my exit status ");
        scanf(" %d", &rv);
        exit(rv);
    default:
        printf("this is a parent process\n");
        printf("pid = %d\n", getpid());
        printf("my child's pid is %d\n", pid);
        printf("waiting for my child to exit()...\n");
        wait(&rv);
        printf("my child's exit status is: %d\n", WEXITSTATUS(rv));
    }
    return 0;
}
