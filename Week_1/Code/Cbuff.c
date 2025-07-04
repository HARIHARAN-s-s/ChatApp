#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // For bool type

// Define the size of the circular buffer
#define BUFFER_SIZE 5

// Structure to represent the circular buffer
typedef struct {
    int data[BUFFER_SIZE]; // Array to store data
    int head;              // Index of the first (oldest) element
    int tail;              // Index of the next available slot for a new element
    int count;             // Number of elements currently in the buffer
} CircularBuffer;

// Function to initialize the circular buffer
void initCircularBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Function to check if the circular buffer is empty
bool isBufferEmpty(CircularBuffer *cb) {
    return cb->count == 0;
}

// Function to check if the circular buffer is full
bool isBufferFull(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
}

// Function to add an element to the circular buffer
void enqueue(CircularBuffer *cb, int value) {
    if (isBufferFull(cb)) {
        printf("Buffer is full. Cannot enqueue %d.\n", value);
        return;
    }
    cb->data[cb->tail] = value;
    cb->tail = (cb->tail + 1) % BUFFER_SIZE; // Wrap around if tail reaches end
    cb->count++;
    printf("Enqueued: %d\n", value);
}

// Function to remove an element from the circular buffer
int dequeue(CircularBuffer *cb) {
    if (isBufferEmpty(cb)) {
        printf("Buffer is empty. Cannot dequeue.\n");
        return -1; // Indicate an error or invalid value
    }
    int value = cb->data[cb->head];
    cb->head = (cb->head + 1) % BUFFER_SIZE; // Wrap around if head reaches end
    cb->count--;
    printf("Dequeued: %d\n", value);
    return value;
}

// Function to peek at the front element without removing it
int peek(CircularBuffer *cb) {
    if (isBufferEmpty(cb)) {
        printf("Buffer is empty. Cannot peek.\n");
        return -1;
    }
    return cb->data[cb->head];
}

// Function to print the contents of the circular buffer
void printBuffer(CircularBuffer *cb) {
    if (isBufferEmpty(cb)) {
        printf("Buffer is empty.\n");
        return;
    }
    printf("Buffer contents (head to tail): ");
    int i;
    for (i = 0; i < cb->count; i++) {
        printf("%d ", cb->data[(cb->head + i) % BUFFER_SIZE]);
    }
    printf("\n");
}

int main() {
    CircularBuffer myBuffer;
    initCircularBuffer(&myBuffer);

    printBuffer(&myBuffer);

    enqueue(&myBuffer, 10);
    enqueue(&myBuffer, 20);
    enqueue(&myBuffer, 30);
    printBuffer(&myBuffer);

    dequeue(&myBuffer);
    printBuffer(&myBuffer);

    enqueue(&myBuffer, 40);
    enqueue(&myBuffer, 50);
    enqueue(&myBuffer, 60); // This will attempt to enqueue when full
    printBuffer(&myBuffer);

    printf("Peeking at front: %d\n", peek(&myBuffer));

    while (!isBufferEmpty(&myBuffer)) {
        dequeue(&myBuffer);
    }
    dequeue(&myBuffer); // This will attempt to dequeue when empty
    printBuffer(&myBuffer);

    return 0;
}
