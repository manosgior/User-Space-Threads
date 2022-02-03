#include <stdlib.h>
#include "queue.h"
#include "scheduler.h"
#include "thread.h"
#include "wrappers.h"
#include "sem.h"

#define STACK_SIZE 16384

typedef enum {active, blocked, finished} State;

typedef struct Thread {
    unsigned long   *stack;
    int             tid;
    int             return_code;
    int             to_join;
    State           state;
} Thread;

int nthreads = 0;
Thread mainThread;
Thread *running = NULL;
Queue runQueue;
Queue blockedQueue;

int _STARTMONITOR() {volatile int x = 0; x++; return x;}
extern void _ENDMONITOR();
extern void _swtch(void *from, void *to);
extern void _thrstart();
extern int flag;

void _switch() {
    Thread *current = running;
    running = (Thread *) dequeue(runQueue);    

    if (running != NULL)
        _swtch(current, running);
}

int sig_handler(struct sigcontext sc) { 
    if (flag == 1 || (sc.eip >= _STARTMONITOR && sc.eip <= _ENDMONITOR))
        return 0;
    
    enqueue(running, runQueue);
    my_sigsetmask(sc.oldmask);
    _switch();

    return 0;
}

void Thread_init() {
    mainThread.state = active;
    mainThread.tid = ++nthreads;
    running = &mainThread;
    runQueue = init_queue(runQueue);
    blockedQueue = init_queue(blockedQueue);
    scheduler_init(sig_handler);
}

int Thread_self() {
    if (running != NULL)
        return running->tid;
    else {
        print_error("Error: there is no thread currently running. Exiting...\n");
        exit(-1);
    }
}

void Thread_pause() {
    if (running != NULL) {
        enqueue(running, runQueue);
        _switch();
    }       
    else {
        print_error("Error: there is no thread currently running. Exiting...\n");
        exit(-1);
    }
}

int Thread_new(int func(void *), void *args, size_t nbytes, ...){
    if (running == NULL) {
        print_error("Error: \"Thread_init\" not called. Exiting...\n");
        exit(-1);
    }
    else if (func == NULL) {
        print_error("Error: NULL function pointer passed to Thread_new. Exiting...\n");
        return -1;
    }
    else if (args != NULL && nbytes < 0) {
        print_error("Error: non NULL \"args\" but 0 \"nbytes\" passed to \"Thread_new\". Exiting...\n");
        return -1;
    }
    else {
        Thread *new = my_malloc(sizeof (struct Thread));
        if (new == NULL) {
            print_error("Out of Memory. Exiting...\n");
            return -1;
        }
        new->stack = my_malloc(STACK_SIZE);
        if (new->stack == NULL) {
            print_error("Out of Memory. Exiting...\n");
            return -1;
        }
        new->state = active;
        new->tid = ++nthreads;        
        new->stack += STACK_SIZE;

        void *newArgsLocation = args;
        if (nbytes > 0){
            newArgsLocation = my_malloc(nbytes);
            if (newArgsLocation == NULL) {
                print_error("Out of Memory. Exiting...\n");
                return -1;
            }
            
            my_memcpy(newArgsLocation, args, nbytes);
        }
        
        new->stack -= 5;
        new->stack[1] = func;
        new->stack[2] =  newArgsLocation;
        new->stack[4] = _thrstart;
      
        enqueue(new, runQueue);      
        
        return new->tid;
    }
}

void Thread_exit(int code) {
    if (running == NULL) {
        print_error("Error: \"Thread_init\" not called. Exiting...\n");
        exit(-1);
    }
    running->state = finished;
    Thread *first  = (Thread *)front(blockedQueue);
    Thread *tmp = (Thread *)dequeue(blockedQueue);

    while (tmp != NULL) {
        if (tmp->to_join == running->tid) {           
            tmp->return_code = code;    
            tmp->state = active;
            enqueue(tmp, runQueue);                                                      
        }
        else if(tmp->to_join == 0 && nthreads == 2)
            enqueue(tmp, runQueue);
        else
            enqueue(tmp, blockedQueue);
        
        tmp = (Thread *)front(blockedQueue);
        if (tmp == first)             
            break;      
        else
            tmp = (Thread *) dequeue(blockedQueue);        
    }   

    nthreads--;

    if (nthreads >= 1) 
        _switch();
    else 
        exit(0);
}

int Thread_join(int tid) {
    if (running == NULL) {
        print_error("Error: \"Thread_init\" not called. Exiting...\n");
        exit(-1);
    }  
    else if (running->tid == tid) {
        print_error("Error: Thread calling \"Thread_join\" and thread descripted by tid are the same. Exiting...\n");
        exit(-1);
    }

    if (tid > 0) {
        Thread *first = (Thread *)front(runQueue);
        Thread *tmp = (Thread *)dequeue_enqueue(runQueue);

        do  {
            if (tmp->tid == tid) {
                if (tmp->state == finished)
                    return -1;
                running->state = blocked;
                enqueue(running, blockedQueue);
                _switch();
                return running->return_code;
            }
            tmp = (Thread *)dequeue_enqueue(runQueue);
        }while(tmp != first);

        return -1;
    }
    else if (tid == 0) {
        running->state = blocked;
        running->to_join = 0;
        enqueue(running, blockedQueue);        
        
        _switch();

        return 0;
    }
    else {
        print_error("Error: \"Thread_init\" called with negative tid. Exiting...\n");
        exit(-1);
    }

}

void Sem_init(Sem_T *s, int count) {
    s->count = count;
    init_queue(s->queue);
}

void Sem_wait(Sem_T *s) {
    if (s->count > 0)
        s->count--;
        return;

    enqueue(running, s->queue);
    _switch();
}

void Sem_signal(Sem_T *s) {
    if (isEmpty_queue(s->queue))
        s->count++;
    else {
        enqueue(dequeue(s->queue), running);
    }
}