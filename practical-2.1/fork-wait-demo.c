// program1_sort.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // fork(), getpid(), getppid()
#include <sys/wait.h>  // wait()

#define MAX 10

// ── Bubble Sort (used by CHILD) ─────────────────────────────
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp     = arr[j];
                arr[j]   = arr[j + 1];
                arr[j+1] = temp;
            }
        }
    }
}

// ── Insertion Sort (used by PARENT) ────────────────────────
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j   = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[MAX], n, i;
    pid_t pid;

    // ── Step 1: Accept input in parent ──────────────────────
    printf("Enter number of integers: ");
    scanf("%d", &n);
    printf("Enter %d integers: ", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("\nOriginal array: ");
    printArray(arr, n);

    // ── Step 2: fork() ──────────────────────────────────────
    pid = fork();

    if (pid < 0) {
        // fork failed
        perror("fork failed");
        exit(1);
    }

    // ── Step 3: CHILD — Bubble Sort ─────────────────────────
    else if (pid == 0) {
        printf("\n[CHILD]  PID=%d  PPID=%d\n", getpid(), getppid());
        printf("[CHILD]  Sorting using Bubble Sort...\n");

        bubbleSort(arr, n);  // works on its OWN copy of arr

        printf("[CHILD]  Sorted array: ");
        printArray(arr, n);
        printf("[CHILD]  Done. Exiting.\n");
        exit(0);             // child exits cleanly
    }

    // ── Step 4: PARENT — Insertion Sort + wait() ────────────
    else {
        printf("\n[PARENT] PID=%d  Child PID=%d\n", getpid(), pid);
        printf("[PARENT] Sorting using Insertion Sort...\n");

        insertionSort(arr, n);  // works on its OWN copy of arr

        printf("[PARENT] Sorted array: ");
        printArray(arr, n);

        // wait() blocks parent here until child finishes
        // this prevents the child from becoming a zombie
        int status;
        wait(&status);

        if (WIFEXITED(status))
            printf("[PARENT] Child exited with status: %d\n", WEXITSTATUS(status));

        printf("[PARENT] Done.\n");
    }

    return 0;
}
