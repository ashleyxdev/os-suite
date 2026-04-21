#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // getpid()

int main(int argc, char *argv[]) {

    // ── 1. Validate arguments ─────────────────────────────────────────
    if (argc < 2) {
        printf("No array received. Run via main.\n");
        return 1;
    }

    // ── 2. Rebuild array from argv[] ──────────────────────────────────
    int n = atoi(argv[1]);

    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 2]);
    }

    // ── 3. Print received array ───────────────────────────────────────
    printf("\n[Search - PID: %d] Received sorted array: ", getpid());
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    // ── 4. Accept search target ───────────────────────────────────────
    int target;
    printf("[Search] Enter element to search: ");
    scanf("%d", &target);

    // ── 5. Binary search ──────────────────────────────────────────────
    int low = 0, high = n - 1, mid = 0, found = 0;

    while (low <= high) {
        mid = (low + high) / 2;

        if (arr[mid] == target) {
            found = 1;
            break;
        }
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    // ── 6. Result ─────────────────────────────────────────────────────
    if (found)
        printf("[Search] %d found at index %d.\n", target, mid);
    else
        printf("[Search] %d not found in the array.\n", target);

    return 0;
}
