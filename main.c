#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 20 // Size of the shared buffer
#define EMPTY 0        // Constant indicating an empty buffer
#define FULL 20        // Constant indicating a full buffer
#define PRODUCER_NUM 4 // Number of producers
#define CONSUMER_NUM 4 // Number of consumers

// Mutex and semaphore for synchronizing access to the buffer
pthread_mutex_t buffer_mutex;
sem_t buffer_sem;

// Shared buffer and its size tracker
int size = 0;
int buffer[BUFFER_SIZE];

// Function declarations for consumer and producer routines
void *consumerRoutine(void *args);
void *producerRoutine(void *argd);
void printBuffer(int size);

int main() {
  // Initialize the semaphore and mutex for buffer synchronization
  sem_init(&buffer_sem, 0, 1);
  pthread_mutex_init(&buffer_mutex, NULL);

  // Create threads for the producer and consumer
  pthread_t the_consumer_thread, the_producer_thread;
  pthread_create(&the_consumer_thread, NULL, consumerRoutine,
                 NULL); // Consumer thread
  pthread_create(&the_producer_thread, NULL, producerRoutine,
                 NULL); // Producer thread

  // Wait for both threads to finish
  pthread_join(the_producer_thread, NULL);
  pthread_join(the_consumer_thread, NULL);

  // Destroy the mutex and semaphore after thread execution
  pthread_mutex_destroy(&buffer_mutex);
  sem_destroy(&buffer_sem);

  return 0;
}

// Consumer routine: consumes values from the buffer
void *consumerRoutine(void *args) {
  while (1) {
    // Check if the buffer is empty
    if (size - 1 == EMPTY) {
      printf("Buffer is empty, consumer is waiting for the producer to produce "
             "items.\n");
      sem_wait(&buffer_sem); // Wait for the buffer to have items
    }

    // Consume items from the buffer as long as it is not empty
    while (size - 1 > EMPTY) {
      pthread_mutex_lock(&buffer_mutex); // Lock the buffer access
      printf("Consumer removed value %d from index %d.\n", buffer[size - 1],
             size - 1);
      buffer[size - 1] = 0; // Reset the consumed value in the buffer
      size--;               // Decrease buffer size
      pthread_mutex_unlock(&buffer_mutex); // Unlock buffer access
      sem_post(&buffer_sem);               // Signal the buffer is available
      sleep(rand() % 4 + 1);               // Simulate consumption time
    }
  }
  return NULL;
}

// Producer routine: produces values and adds them to the buffer
void *producerRoutine(void *args) {
  while (1) {
    // Check if the buffer is full
    if (size == FULL) {
      printf("Buffer is full, producer is waiting for the consumer to consume "
             "items.\n");
      sem_wait(&buffer_sem); // Wait for the buffer to have space
    }

    // Add items to the buffer as long as it is not full
    while (size < FULL) {
      pthread_mutex_lock(&buffer_mutex); // Lock the buffer access
      int newValue =
          rand() % 5 + 1;      // Generate a random value to add to the buffer
      buffer[size] = newValue; // Add new value to the buffer
      size++;                  // Increase buffer size
      printf("Producer added value %d to index %d.\n", newValue, size - 1);
      pthread_mutex_unlock(&buffer_mutex); // Unlock buffer access
      sem_post(&buffer_sem);               // Signal the buffer is available
      sleep(rand() % 2 + 1);               // Simulate production time
    }
  }
  return NULL;
}

// Helper function to print the current state of the buffer
void printBuffer(int size) {
  for (int i = 0; i <= size; i++) {
    printf("[%d]", buffer[i]);
  }
  printf("\n");
}
