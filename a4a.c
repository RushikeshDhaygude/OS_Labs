// Assignment 4 A

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
sem_t empty, full;
pthread_mutex_t mutex;
int in = 0, out = 0;

// Function to print the current state of the buffer
void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", buffer[i]);
        if (i < BUFFER_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Producer thread function
void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100;

        // Wait for an empty slot in the buffer
        sem_wait(&empty);

        // Acquire the mutex to update the buffer
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d Produced: %d\n", in, item);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);

        // Signal that a slot in the buffer is full
        sem_post(&full);

        // Print the current state of the buffer
        print_buffer();

        // Sleep for 2 seconds (simulating work)
        sleep(2);
    }
    pthread_exit(NULL);
}

// Consumer thread function
void *consumer(void *arg) {
    int item;
    while (1) {
        // Wait for a full slot in the buffer
        sem_wait(&full);

        // Acquire the mutex to access the buffer
        pthread_mutex_lock(&mutex);
        item = buffer[out];
        printf("Consumer %d Consumed: %d\n", out, item);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);

        // Signal that a slot in the buffer is empty
        sem_post(&empty);

        // Print the current state of the buffer
        print_buffer();

        // Sleep for 5 seconds (simulating work)
        sleep(5);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores and mutex
    //sem_destroy(&empty);
    //sem_destroy(&full);
    //pthread_mutex_destroy(&mutex);
   

    return 0;
}

