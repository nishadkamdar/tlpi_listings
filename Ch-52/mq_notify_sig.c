#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define NOTIFY_SIG SIGUSR1

static void handler(int sig)
{
	
}

int main (int argc, char *argv[])
{
	struct sigevent sev;
	mqd_t mqd;
	struct mq_attr attr;
	void *buffer;
	ssize_t num_read;
	sigset_t block_mask, empty_mask;
	struct sigaction sa;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
	{
		printf("%s mq-name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	if (mqd == (mqd_t) - 1)
	{
		printf("mq_open error\n");
		exit(EXIT_FAILURE);
	}

	if (mq_getattr(mqd, &attr) == -1)
	{
		printf("mq_getattr error\n");
		exit(EXIT_FAILURE);
	}

	buffer = malloc(attr.mq_msgsize);
	if (buffer == NULL)
	{
		printf("malloc error\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&block_mask);
	sigaddset(&block_mask, NOTIFY_SIG);
	if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1)
	{
		printf("sigprocmask\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(NOTIFY_SIG, &sa, NULL) == -1)
	{
		printf("sigaction \n");
		exit(EXIT_FAILURE);
	}

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	if (mq_notify(mqd, &sev) == -1)
	{
		printf("mq_notify error\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&empty_mask);

	for(;;)
	{
		sigsuspend(&empty_mask);

		if (mq_notify(mqd, &sev) == -1)
		{
			printf("mq_notify error\n");
			exit(EXIT_FAILURE);
		}

		while((num_read = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
			printf("Read %ld bytes\n", (long) num_read);

		if (errno != EAGAIN)
		{
			printf("mq_receive error\n");
			exit(EXIT_FAILURE);
		}
	}
}
