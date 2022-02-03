#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <signal.h>
#include <sys/time.h>
//#include <asm/sigcontext.h>

void scheduler_init(void(*handler)(int));

#endif