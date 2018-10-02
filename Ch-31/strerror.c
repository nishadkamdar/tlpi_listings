#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ERROR_LEN 256

static char buf[MAX_ERROR_LEN];

static void *thread_func(void *arg)
{
	char *str;

	printf("Other thread about to call strerror()\n");
	str = strerror(EPERM);
	printf("Other thread: str (%p) = %s\n", str, str);

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t t;
	int s;
	char *str;

	str = strerror(EINVAL);
	printf("Main thread has called strerror()\n");

	s = pthread_create(&t, NULL, thread_func, NULL);
	if (s != 0)
	{
		printf("pthread_create error\n");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t, NULL);
	if (s != 0)
	{
		printf("pthread_join error\n");
		exit(EXIT_FAILURE);
	}

	printf("Main thread: str (%p) = %s\n", str, str);

	exit(EXIT_SUCCESS);
}

char *strerror(int err)
{
	if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL)
	{
		snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
	}
	else
	{
		strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
		buf[MAX_ERROR_LEN - 1] = '\0';
	}

	return buf;
}
