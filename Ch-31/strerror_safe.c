#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerror_key;

#define MAX_ERROR_LEN 256

static void destructor(void *buf)
{
	free(buf);
}

static void create_key(void)
{
	int s;

	s = pthread_key_create(&strerror_key, destructor);
	if (s != 0)
	{
		printf("pthread_key_create\n");
		exit(EXIT_FAILURE);
	}
}

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
	int s;
	char  *buf;

	s = pthread_once(&once, create_key);
	if(s != 0)
	{
		printf("pthread_once_error\n");
		exit(EXIT_FAILURE);

	}

	buf = pthread_getspecific(strerror_key);
	if (buf == NULL)
	{
		buf = malloc(MAX_ERROR_LEN);
		if (buf == NULL)
		{
			printf("error executing malloc\n");
			exit(EXIT_FAILURE);
		}

		s = pthread_setspecific(strerror_key, buf);
		if (s != 0)
		{
			printf("pthread_setspecific\n");
			exit(EXIT_FAILURE);
		}
	}

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
