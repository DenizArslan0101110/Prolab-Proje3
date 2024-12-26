#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdlib.h>
#include <limits.h>
#include "Structs.h"

struct Queue* CreateQueue(int capacity);
void Enqueue(struct Queue* queue, int item);
int Dequeue(struct Queue* queue);

#endif // QUEUE_H_INCLUDED
