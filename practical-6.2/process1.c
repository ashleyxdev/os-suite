#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO1    "fifo1"
#define FIFO2    "fifo2"
#define BUF_SIZE 4096

int main() {

    // ── Accept sentences from user ────────────────────────────────────
    char input[BUF_SIZE] = "";
    char line[256];

    printf("[Process1] Enter sentences (empty line to finish):\n");

    while (1) {
        fgets(line, sizeof(line), stdin);
        if (strcmp(line, "\n") == 0)   // empty line = done
            break;
        strcat(input, line);
    }

    // ── Open fifo1 for writing (blocks until process2 opens for read)
    int fd1 = open(FIFO1, O_WRONLY);
    if (fd1 == -1) { perror("open fifo1 failed"); exit(1); }

    write(fd1, input, strlen(input));
    close(fd1);
    printf("[Process1] Sentences sent to process2.\n");

    // ── Open fifo2 for reading (blocks until process2 opens for write)
    int fd2 = open(FIFO2, O_RDONLY);
    if (fd2 == -1) { perror("open fifo2 failed"); exit(1); }

    // ── Read and display result ───────────────────────────────────────
    char result[BUF_SIZE];
    int bytes = read(fd2, result, sizeof(result) - 1);
    result[bytes] = '\0';
    close(fd2);

    printf("\n[Process1] Analysis received from process2:\n");
    printf("─────────────────────────────────\n");
    printf("%s", result);
    printf("─────────────────────────────────\n");

    return 0;
}
