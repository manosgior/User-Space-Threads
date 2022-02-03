#ifndef SEM_H
#define SEM_H
#include "queue.h"

typedef struct Sem_T {      
    int count;
    Queue *queue;
} Sem_T;

void Sem_init(Sem_T *s, int count);
void Sem_wait(Sem_T *s);
void Sem_signal(Sem_T *s);

#endif

