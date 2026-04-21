// program3_orphan.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    // ── CHILD: keeps running after parent dies ───────────────
    else if (pid == 0) {
        printf("[CHILD]  PID=%d  PPID before parent exits = %d\n",
               getpid(), getppid());

        sleep(5);  // give parent time to exit first

        // After parent dies, PPID changes to 1 (init/systemd)
        printf("[CHILD]  PID=%d  PPID after parent exits  = %d\n",
               getpid(), getppid());

        printf("[CHILD]  I am now an ORPHAN. Adopted by init (PID=1).\n");
        sleep(5);
        printf("[CHILD]  Orphan exiting.\n");
        exit(0);
    }

    // ── PARENT: exits immediately, before child finishes ─────
    else {
        printf("[PARENT] PID=%d  Child PID=%d\n", getpid(), pid);
        printf("[PARENT] I am exiting NOW, before child finishes.\n");
        // no wait() — parent just dies
        exit(0);
    }

    return 0;
}
