#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO1     "fifo1"
#define FIFO2     "fifo2"
#define OUTFILE   "output.txt"
#define BUF_SIZE  4096

int main() {

    // ── Create both FIFOs ─────────────────────────────────────────────
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
    printf("[Process2] FIFOs created. Waiting for process1...\n");

    // ── Open fifo1 for reading (blocks until process1 opens for write)
    int fd1 = open(FIFO1, O_RDONLY);
    if (fd1 == -1) { perror("open fifo1 failed"); exit(1); }
    printf("[Process2] Connected to fifo1. Reading sentences...\n");

    // ── Read all sentences from process1 ─────────────────────────────
    char input[BUF_SIZE];
    int bytes = read(fd1, input, sizeof(input) - 1);
    input[bytes] = '\0';
    close(fd1);

    printf("[Process2] Received:\n%s\n", input);

    // ── Count characters, words, lines ───────────────────────────────
    int chars = 0, words = 0, lines = 0;
    int in_word = 0;

    for (int i = 0; i < bytes; i++) {
        chars++;

        if (input[i] == '\n')
            lines++;

        if (input[i] == ' '  ||
            input[i] == '\n' ||
            input[i] == '\t') {
            in_word = 0;
        } else {
            if (!in_word) {
                words++;
                in_word = 1;
            }
        }
    }

    printf("[Process2] chars=%d  words=%d  lines=%d\n",
           chars, words, lines);

    // ── Write result to output.txt ────────────────────────────────────
    FILE *fp = fopen(OUTFILE, "w");
    fprintf(fp, "Characters : %d\n", chars);
    fprintf(fp, "Words      : %d\n", words);
    fprintf(fp, "Lines      : %d\n", lines);
    fclose(fp);

    printf("[Process2] Results written to %s\n", OUTFILE);

    // ── Open fifo2 for writing (blocks until process1 opens for read)
    int fd2 = open(FIFO2, O_WRONLY);
    if (fd2 == -1) { perror("open fifo2 failed"); exit(1); }

    // ── Read output.txt and send to process1 via fifo2 ───────────────
    fp = fopen(OUTFILE, "r");
    char line[256];
    char result[BUF_SIZE] = "";

    while (fgets(line, sizeof(line), fp) != NULL)
        strcat(result, line);
    fclose(fp);

    write(fd2, result, strlen(result));
    close(fd2);

    printf("[Process2] Results sent to process1. Done.\n");

    // ── Cleanup FIFOs ─────────────────────────────────────────────────
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
