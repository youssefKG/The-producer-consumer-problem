# Producer-Consumer Multithreaded Program

This project is a C implementation of the Producer-Consumer problem using threads, mutexes, and semaphores. It simulates multiple producer and consumer threads that interact with a shared buffer. The producers add items to the buffer, and the consumers remove them. Mutex locks and semaphores are used to manage concurrent access to the buffer and prevent race conditions.

## Features

- **Multithreading**: Uses `pthread` library to create and manage multiple producer and consumer threads.
- **Concurrency Control**: Implements mutexes and semaphores to synchronize access to a shared buffer.
- **Buffer Management**: Producers and consumers interact with a fixed-size buffer, ensuring no overflows or underflows occur.

## How It Works

- **Producers**: Each producer thread generates a random value and adds it to the shared buffer if there is space.
- **Consumers**: Each consumer thread removes values from the buffer if it is not empty.
- **Synchronization**: 
  - A **mutex** (`buffer_mutex`) ensures only one thread can access the buffer at a time.
  - A **semaphore** (`buffer_sem`) ensures that producers and consumers wait when the buffer is full or empty, respectively.

## Compilation and Execution

### Prerequisites

- GCC (or any C compiler)
- POSIX thread library (`pthread`)

### Steps to Compile and Run

1. **Clone the repository** or copy the source files to your machine.
2. **Compile the program**:
   ```bash
   gcc -pthread producer_consumer.c -o producer_consumer

