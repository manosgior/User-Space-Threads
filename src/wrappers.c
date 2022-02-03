#include "wrappers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int flag = 0;

void *my_malloc(unsigned int size) {
    void *toReturn;
    flag = 1;
    toReturn = aligned_alloc(sizeof (void *), size);
    flag = 0;

    return toReturn;
}

void print_error(const char *format){
    flag = 1;
    fprintf(stderr, format);
    flag = 0;
}

void *my_memcpy(void *destination, const void *source, size_t num) {
    flag = 1;
    memcpy(destination, source, num);
    flag = 0;

    return destination;
}

void my_sigsetmask(int mask) {
    flag = 1;
    sigsetmask(mask);
    flag = 0;
}