#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   10

// ── Shared buffer ─────────────────────────────────────────────────────
int buffer[BUFFER_SIZE];
int in  = 0;   // next empty slot  (producer writes here)
int out = 0;   // next filled slot (consumer reads here)

// ── Synchronization tools ─────────────────────────────────────────────
sem_t empty;              // counts empty slots
sem_t full;               // counts filled slots
pthread_mutex_t mutex;    // protects buffer access

// ── Producer thread ───────────────────────────────────────────────────
void *producer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {

        int item = i;   // item being produced

        sem_wait(&empty);           // wait for an empty slot
        pthread_mutex_lock(&mutex); // lock the buffer

        // ── critical section ──
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;   // circular buffer
        printf("[Producer] Produced item: %d | buffer[in] at slot %d\n",
               item, (in - 1 + BUFFER_SIZE) % BUFFER_SIZE);
        // ── end critical section ──

        pthread_mutex_unlock(&mutex); // unlock
        sem_post(&full);              // signal one more item available

        sleep(1);   // simulate production time
    }
    return NULL;
}

// ── Consumer thread ───────────────────────────────────────────────────
void *consumer(void *arg) {
    for (int i = 1; i <= NUM_ITEMS; i++) {

        sem_wait(&full);            // wait for a filled slot
        pthread_mutex_lock(&mutex); // lock the buffer

        // ── critical section ──
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;  // circular buffer
        printf("[Consumer] Consumed item: %d | buffer[out] at slot %d\n",
               item, (out - 1 + BUFFER_SIZE) % BUFFER_SIZE);
        // ── end critical section ──

        pthread_mutex_unlock(&mutex); // unlock
        sem_post(&empty);             // signal one more slot free

        sleep(2);   // consumer is slower than producer
    }
    return NULL;
}

// ── Main ──────────────────────────────────────────────────────────────
int main() {

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // all slots empty at start
    sem_init(&full,  0, 0);            // no items at start
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\nAll items produced and consumed. Exiting.\n");
    return 0;
}
