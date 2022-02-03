
#ifndef THREAD_H
#define THREAD_H

#include <stddef.h>

void Thread_init(void);
int Thread_new(int func(void *), void *args, size_t nbytes, ...);
void Thread_exit(int code);
int Thread_self(void);
int Thread_join(int tid);
void Thread_pause(void);

#endif

