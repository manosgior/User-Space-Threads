The goal of this simple project was to implement a structured multitasking facility - `threads' - in Unix by multiplexing a single process. The facility and its implementation are similar to one described in `A micro-kernel for concurrency in C'.

The header thread.h is an interface for the set of thread primitives described in the thread (3) man page. Programs that use threads can also use semaphores and channels for synchronization and communication.  
sem.h is the interface for the semaphore primitives described in the sem (3) man page, and chan.h is the interface for the CSP-style channel primitives described in the chan (3) man page.  

Implemented all queues as first-in, first-out queues. Implemented extern void _swtch(void *from, void *to); from [this](https://www.csd.uoc.gr/~hy527/as1/swtch.s) template but for x86. 
