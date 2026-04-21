#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    // ── 1. Accept input ───────────────────────────────────────────────
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) {
        printf("Enter element %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // ── 2. Fork ───────────────────────────────────────────────────────
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    // ── 3. Child Process ──────────────────────────────────────────────
    if (pid == 0) {
        // Child does nothing — it will be replaced by execve from parent
        // Parent will wait() for this exit before calling execve
        printf("[Child  - PID: %d] Created. Exiting cleanly.\n", getpid());
        exit(0);
    }

    // ── 4. Parent Process ─────────────────────────────────────────────
    else {

        printf("[Parent - PID: %d] Sorting the array...\n", getpid());

        // Bubble sort
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp   = arr[j];
                    arr[j]     = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        printf("[Parent - PID: %d] Sorted array: ", getpid());
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        // Wait for child to finish before replacing itself
        wait(NULL);
        printf("[Parent - PID: %d] Child finished. Loading search program...\n", getpid());

        // ── 5. Build argv[] for execve ────────────────────────────────
        // args[0] = "./search"   (program name)
        // args[1] = n            (array size, as string)
        // args[2..n+1] = sorted elements (each as string)
        // args[n+2] = NULL       (required terminator)

        char *args[n + 3];
        args[0] = "./search";

        char size_str[10];
        sprintf(size_str, "%d", n);
        args[1] = size_str;

        char num_strs[n][20];
        for (int i = 0; i < n; i++) {
            sprintf(num_strs[i], "%d", arr[i]);
            args[i + 2] = num_strs[i];
        }
        args[n + 2] = NULL;

        // ── 6. Replace this process with search program ───────────────
        execve("./search", args, NULL);

        // Only reached if execve fails
        perror("execve failed");
        exit(1);
    }

    return 0;
}
