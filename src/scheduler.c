#include "scheduler.h"
#include <stdlib.h>
#include <string.h>

#define TIME_INTERVAL 100

void set_timer() {
    struct itimerval tv;
    tv.it_interval.tv_sec = 0;
    tv.it_interval.tv_usec = TIME_INTERVAL;
    tv.it_value.tv_sec = 0;
    tv.it_value.tv_usec = TIME_INTERVAL;

    setitimer(ITIMER_VIRTUAL, &tv, NULL);
}

void set_signal(void(*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handler;
    sigaction(SIGVTALRM, &sa, NULL);
}

void scheduler_init(void(*handler)(int)) {
    set_timer();
    set_signal(handler);
}
