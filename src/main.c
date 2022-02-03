#include <stdio.h>
#include "thread.h"

int counter = 0;

int tester(void *args) {	
	int x = *(int *) args;

	for(int i = 0; i < 10; i++) {
		counter++;		
	}
}

int main(int argc, char *argv[]) {
	int names[] = {1,2,3,4,5};
	Thread_init();
	for (int i = 0; i < 5; i++) {
		Thread_new(tester, &names[i], sizeof(int));
	}	
	Thread_join(0);
	printf("counter=%d\n", counter);
	Thread_exit(0);
}