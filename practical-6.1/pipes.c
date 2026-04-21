#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {

    // ── Create two pipes ──────────────────────────────────────────────
    int pipe1[2];   // parent → child  (filename)
    int pipe2[2];   // child  → parent (file contents)

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // ── Fork ──────────────────────────────────────────────────────────
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    // ── Child Process ─────────────────────────────────────────────────
    if (pid == 0) {

        // Child reads from pipe1, writes to pipe2
        // Close unused ends
        close(pipe1[1]);   // child does not write to pipe1
        close(pipe2[0]);   // child does not read from pipe2

        // Read filename from parent
        char filename[256];
        int bytes = read(pipe1[0], filename, sizeof(filename) - 1);
        filename[bytes] = '\0';
        close(pipe1[0]);   // done reading

        printf("[Child] Received filename: %s\n", filename);

        // Open and read the file
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            char *err = "ERROR: File not found\n";
            write(pipe2[1], err, strlen(err));
            close(pipe2[1]);
            exit(1);
        }

        // Send file contents to parent through pipe2
        char line[BUFFER_SIZE];
        while (fgets(line, sizeof(line), fp) != NULL) {
            write(pipe2[1], line, strlen(line));
        }

        fclose(fp);
        close(pipe2[1]);   // done writing — parent will get EOF
        printf("[Child] File contents sent to parent.\n");
        exit(0);
    }

    // ── Parent Process ────────────────────────────────────────────────
    else {

        // Parent writes to pipe1, reads from pipe2
        // Close unused ends
        close(pipe1[0]);   // parent does not read from pipe1
        close(pipe2[1]);   // parent does not write to pipe2

        // Accept filename from user
        char filename[256];
        printf("[Parent] Enter filename to read: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';  // strip newline

        // Send filename to child
        write(pipe1[1], filename, strlen(filename));
        close(pipe1[1]);   // done writing
        printf("[Parent] Filename sent to child.\n");

        // Wait for child to send contents
        printf("[Parent] File contents received:\n");
        printf("─────────────────────────────────\n");

        char buffer[BUFFER_SIZE];
        int bytes;
        while ((bytes = read(pipe2[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes] = '\0';
            printf("%s", buffer);
        }

        printf("─────────────────────────────────\n");
        close(pipe2[0]);

        wait(NULL);
        printf("[Parent] Child process finished.\n");
    }

    return 0;
}


// echo -e "Hello from the file\nThis is line 2\nThis is line 3" > test.txt
// gcc pipes.c -o pipes
// ./pipes
