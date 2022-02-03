#include "queue.h"
#include <stdlib.h>

struct node {
    void *data;
    struct node *next;
};

struct queue {
    struct node *front;
    struct node *back;
};

Queue init_queue() {
    Queue toReturn = malloc(sizeof(struct queue));
    toReturn->front = toReturn->back = NULL;

    return toReturn;
}

int isEmpty_queue(Queue q) {
    if (q->front == NULL)
        return 1;
    else return 0;
}

void *front(Queue q) {
    if (isEmpty_queue(q))
        return NULL;
    return q->front->data;
}

void enqueue(void *data, Queue q) {
    struct node *new = malloc(sizeof(struct node));
    new->data = data;
    new->next = NULL;
    if (isEmpty_queue(q))
        q->front = new;
    else
        q->back->next = new;
    q->back = new;
}

void *dequeue(Queue q) {
    if (isEmpty_queue(q))
        return NULL;
    
    struct node *toFree = q->front;
    void *toReturn = toFree->data;

    q->front = q->front->next;
    free(toFree);
    
    if (q->front == NULL)
        q->back = NULL;

    return toReturn;
}

void *dequeue_enqueue(Queue q){
    if (isEmpty_queue(q))
        return NULL;

    void *toReturn = q->front->data;
    struct node *to_enqueue = q->front;
    q->front = q->front->next;

    to_enqueue->next = NULL;    
    q->back->next = to_enqueue;
    q->back = to_enqueue;

    return toReturn;
}