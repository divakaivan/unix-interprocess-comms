#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_handler(int sig) {
    const char msg[] = "Ahh! SIGINT!!\n";
    write(0, msg, sizeof(msg));
    // write is async-safe while printf isnt
    // the handler must be async-safe
}

int main(void) {
    char s[200];
    struct sigaction sa = {
        .sa_handler = sigint_handler,
        .sa_flags = SA_RESTART, // or 0
        .sa_mask = 0,
    };
    // SIGINT is ctrl+C
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    printf("Enter a string:\n");
    if (fgets(s, sizeof s, stdin) == NULL)
        perror("fgets");
    else
        printf("You entered: %s\n", s);
    return 0;
}
