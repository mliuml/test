/*
 * ABCPrint.c
 *
 *  Created on: 2017年11月14日
 *      Author: ming
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#define NUM 3
static int n = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* ThreadPrintId(void* arg)
{
	int id = (int)arg;
	char idname = 'A' + id;

	for (int i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&mutex);
		while (n != id)
		{
#ifdef DEBUG
			printf(" thread %d waiting...\n", id);
#endif
			pthread_cond_wait(&cond, &mutex);
		}

		printf("%c", idname);

		n = (n + 1) % NUM;
		//if (n == 0)
		//	printf("\n");
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&cond);
	}

	return (void*)0;
}

int ABCPrint()
{
	int err;
	pthread_t tid[NUM];
	for (int i = 0; i < NUM; i++)
	{
		err = pthread_create(&tid[i], NULL, ThreadPrintId, (void*)i);
		if (err != 0)
		{
			printf("create thread %d failed: %s\n", i, strerror(err));
		}
	}

	void* ret;
	for (int i = 0; i < NUM; i++)
	{
		err = pthread_join(tid[i], &ret);
		if (err != 0)
		{
			printf("join thread %d failed: %s\n", i, strerror(err));
		}
	}

	return 0;
}
