// reader_writer.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>   // sleep()

// ── Shared state ────────────────────────────────────────────────────────
int shared_data = 0;        // the "database" everyone reads/writes
int read_count  = 0;        // how many readers are currently active

pthread_mutex_t mutex;      // protects read_count itself
pthread_mutex_t rw_mutex;   // actual read/write exclusion lock

// ── Reader thread ────────────────────────────────────────────────────────
void *reader(void *arg) {
    int id = *(int *)arg;

    // ── ENTRY SECTION ──────────────────────────────────────────────────
    pthread_mutex_lock(&mutex);        // lock to safely modify read_count
        read_count++;
        if (read_count == 1) {
            // I am the FIRST reader — block any waiting writer
            pthread_mutex_lock(&rw_mutex);
        }
    pthread_mutex_unlock(&mutex);      // done modifying read_count

    // ── READING (critical section for readers) ──────────────────────────
    printf("[Reader %d] Reading shared_data = %d  (active readers: %d)\n",
           id, shared_data, read_count);
    sleep(1);   // simulate time spent reading

    // ── EXIT SECTION ────────────────────────────────────────────────────
    pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            // I am the LAST reader — allow a writer in
            pthread_mutex_unlock(&rw_mutex);
        }
    pthread_mutex_unlock(&mutex);

    printf("[Reader %d] Done reading.\n", id);
    return NULL;
}

// ── Writer thread ────────────────────────────────────────────────────────
void *writer(void *arg) {
    int id = *(int *)arg;

    // Writers get rw_mutex directly — no read_count involved
    pthread_mutex_lock(&rw_mutex);

    // ── WRITING (critical section) ──────────────────────────────────────
    shared_data++;
    printf("[Writer %d] Wrote shared_data = %d\n", id, shared_data);
    sleep(1);   // simulate time spent writing

    pthread_mutex_unlock(&rw_mutex);

    printf("[Writer %d] Done writing.\n", id);
    return NULL;
}

// ── Main ─────────────────────────────────────────────────────────────────
int main() {
    // Thread IDs (we use separate arrays so each thread gets its own id)
    int r_ids[5] = {1, 2, 3, 4, 5};
    int w_ids[2] = {1, 2};

    pthread_t readers[5], writers[2];

    // Initialise both mutexes
    pthread_mutex_init(&mutex,    NULL);
    pthread_mutex_init(&rw_mutex, NULL);

    printf("=== Reader-Writer Problem: Reader Priority ===\n\n");

    // Spawn 2 writers first (they will be blocked by readers)
    for (int i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer, &w_ids[i]);

    // Small delay so writers arrive first and we can see them get blocked
    usleep(100000);  // 0.1 second

    // Spawn 5 readers — they will proceed even though writers are waiting
    for (int i = 0; i < 5; i++)
        pthread_create(&readers[i], NULL, reader, &r_ids[i]);

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) pthread_join(readers[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(writers[i], NULL);

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&rw_mutex);

    printf("\nFinal shared_data = %d\n", shared_data);
    return 0;
}
