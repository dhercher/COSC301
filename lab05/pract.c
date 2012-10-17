#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include "list.h"

pthread_mutex_t mutex;
void * threadfn(void *arg)
{
	int * i = (int*)arg;
	pthread_mutex_lock(&mutex);
	*i = 43;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char **argv)
{
	int thread_arg = 42;
	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);
	int f = pthread_create(&tid, NULL, threadfn, &thread_arg);
	if (f < 0)
		printf("%s\n", strerror(errno));
	pthread_join(tid, NULL);
	printf("%i\n", thread_arg);
	pthread_mutex_destroy(&mutex);
}