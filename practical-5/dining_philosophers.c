#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINK_TIME       2
#define EAT_TIME         1
#define ROUNDS           3    // how many times each philosopher eats

// ── One semaphore per fork ────────────────────────────────────────────
sem_t forks[NUM_PHILOSOPHERS];

// ── Helper: print state clearly ──────────────────────────────────────
void philosopher_state(int id, const char *state) {
    printf("Philosopher %d is %s\n", id, state);
}

// ── Philosopher thread ────────────────────────────────────────────────
void *philosopher(void *arg) {
    int id = *(int *)arg;

    int left  = id;                        // fork[id]
    int right = (id + 1) % NUM_PHILOSOPHERS;  // fork[(id+1)%5]

    for (int round = 0; round < ROUNDS; round++) {

        // ── THINK ─────────────────────────────────────────────────────
        philosopher_state(id, "thinking...");
        sleep(THINK_TIME);

        // ── PICK UP FORKS ─────────────────────────────────────────────
        if (id == NUM_PHILOSOPHERS - 1) {
            // Last philosopher picks RIGHT first — breaks circular wait
            philosopher_state(id, "hungry (picks RIGHT fork first)");
            sem_wait(&forks[right]);
            philosopher_state(id, "picked up RIGHT fork");
            sem_wait(&forks[left]);
            philosopher_state(id, "picked up LEFT fork");
        } else {
            // All others pick LEFT first
            philosopher_state(id, "hungry (picks LEFT fork first)");
            sem_wait(&forks[left]);
            philosopher_state(id, "picked up LEFT fork");
            sem_wait(&forks[right]);
            philosopher_state(id, "picked up RIGHT fork");
        }

        // ── EAT ───────────────────────────────────────────────────────
        philosopher_state(id, "EATING");
        sleep(EAT_TIME);

        // ── PUT DOWN FORKS ────────────────────────────────────────────
        sem_post(&forks[left]);
        sem_post(&forks[right]);
        philosopher_state(id, "put down forks");
    }

    philosopher_state(id, "DONE");
    return NULL;
}

// ── Main ──────────────────────────────────────────────────────────────
int main() {

    // Initialize all forks as available (value = 1)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        sem_init(&forks[i], 0, 1);

    // Create philosopher threads
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(threads[i], NULL);

    // Cleanup
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
        sem_destroy(&forks[i]);

    printf("\nAll philosophers have eaten %d times. No deadlock.\n", ROUNDS);
    return 0;
}
