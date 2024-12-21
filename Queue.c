#include "Queue.h"

// Function to create a new queue
struct Queue* CreateQueue(int capacity)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = -1;
    queue->data = (int*)malloc(capacity * sizeof(int));
    return queue;
}

// Function to enqueue
void Enqueue(struct Queue* queue, int item)
{
    if (queue->size < queue->capacity)
    {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->data[queue->rear] = item;
        queue->size++;
    }
}

// Function to dequeue
int Dequeue(struct Queue* queue)
{
    if (queue->size > 0)
    {
        int item = queue->data[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return item;
    }
    return -1;
}

