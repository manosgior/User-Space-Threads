#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <string.h>

void *my_malloc(unsigned int size);
void print_error(const char *format);
void *my_memcpy(void *destination, const void *source, size_t num);
void my_sigsetmask(int mask);

#endif