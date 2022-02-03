#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue * Queue;

Queue init_queue();
int isEmpty_queue(Queue q);
void *front(Queue q);
void enqueue(void *data, Queue q);
void *dequeue(Queue q); 
void *dequeue_enqueue(Queue q);

#endif