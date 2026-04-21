// program2_zombie.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    // ── CHILD: exits immediately ─────────────────────────────
    else if (pid == 0) {
        printf("[CHILD]  PID=%d  I am exiting now.\n", getpid());
        exit(0);
        // child is now DEAD, but parent hasn't called wait()
        // → child becomes a ZOMBIE
    }

    // ── PARENT: sleeps without calling wait() ────────────────
    else {
        printf("[PARENT] PID=%d  Child PID=%d\n", getpid(), pid);
        printf("[PARENT] I will sleep for 30 seconds WITHOUT calling wait().\n");
        printf("[PARENT] Open another terminal and run: ps aux | grep Z\n");
        printf("[PARENT] You will see the child as a ZOMBIE (Z state).\n");

        sleep(30);  // parent alive but never waits → child is zombie

        printf("[PARENT] Waking up now. Exiting WITHOUT wait().\n");
        // when parent exits, init adopts and reaps the zombie
    }

    return 0;
}
