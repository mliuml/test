/*
 * GooglePrint.c
 *
 *  Created on: 2017年11月15日
 *      Author: ming
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM 4
#define OUTPUT_END 10

int fd[NUM];

pthread_mutex_t MutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MutexB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MutexC = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MutexD = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t CondA = PTHREAD_COND_INITIALIZER;
pthread_cond_t CondB = PTHREAD_COND_INITIALIZER;
pthread_cond_t CondC = PTHREAD_COND_INITIALIZER;
pthread_cond_t CondD = PTHREAD_COND_INITIALIZER;

int A = 0;
int B = 1;
int C = 2;
int D = 3;

void* ThreadPrintFile(void* arg)
{
	int id = (int)arg;
	char idname = 'A' + id;

	for (int i = 0; i < OUTPUT_END; i++)
	{
		// FileA print id;
		pthread_mutex_lock(&MutexA);
		while (A != id)
		{
			pthread_cond_wait(&CondA, &MutexA);
		}
		write(fd[0], (void*)&idname, 1);

		A = (A + 1) % NUM;
		pthread_mutex_unlock(&MutexA);
		pthread_cond_broadcast(&CondA);

		// FileB print id;
		pthread_mutex_lock(&MutexB);
		while (B != id)
		{
			pthread_cond_wait(&CondB, &MutexB);
		}

		write(fd[1], (void*)&idname, 1);
		B = (B + 1) % NUM;
		pthread_mutex_unlock(&MutexB);
		pthread_cond_broadcast(&CondB);

		// FileC print id;
		pthread_mutex_lock(&MutexC);
		while (C != id)
		{
			pthread_cond_wait(&CondC, &MutexC);
		}

		write(fd[2], (void*)&idname, 1);
		C = (C + 1) % NUM;
		pthread_mutex_unlock(&MutexC);
		pthread_cond_broadcast(&CondC);

		// FileD print id;
		pthread_mutex_lock(&MutexD);
		while (D != id)
		{
			pthread_cond_wait(&CondD, &MutexD);
		}

		write(fd[3], (void*)&idname, 1);
		D = (D + 1) % NUM;
		pthread_mutex_unlock(&MutexD);
		pthread_cond_broadcast(&CondD);
	}
}

int GooglePrint()
{
	char* filename[] = { "./a", "./b", "./c", "./d" };

	for (int i = 0; i < NUM; i++)
	{
		fd[i] = open(filename[i], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		if (fd[i] < 0)
		{
			printf("open file %s failed: %s", filename[i], strerror(errno));
		}
	}

	pthread_t tid[NUM];
	int error;
	for (int i = 0; i < NUM; i++)
	{
		error = pthread_create(&tid[i], NULL, ThreadPrintFile, (void*)i);
		if (error != 0)
		{
			printf("create thread %d failed: %s", i, strerror(error));

		}
	}

	void* ret;
	for (int i = 0; i < NUM; i++)
	{
		error = pthread_join(tid[i], &ret);
		if (error != 0)
		{
			printf("join thread %d failed: %s", i, strerror(error));

		}
	}

	return 0;
}
