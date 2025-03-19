#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        execl("./process", "process", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        sleep(5);
        kill(pid, SIGTSTP);  // Suspend the process
        sleep(10);
        kill(pid, SIGCONT);  // Resume the process
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
