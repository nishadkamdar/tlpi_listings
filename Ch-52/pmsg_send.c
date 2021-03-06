#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-n] name msg [prio]\n", prog_name);
	fprintf(stderr, "	-n		Use O_NONBLOCK flag\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int flags, opt;
	mqd_t mqd;
	unsigned int prio;

	flags = O_WRONLY;
	while ((opt = getopt(argc, argv, "n")) != -1)
	{
		switch(opt)
		{
			case 'n':
				flags |= O_NONBLOCK;
				break;

			default:
				usage_error(argv[0]);
		}
	}

	if (optind + 1 >= argc)
		usage_error(argv[0]);

	mqd = mq_open(argv[optind], flags);
	if (mqd == (mqd_t) - 1)
	{
		printf("mq_open error\n");
		exit(EXIT_FAILURE);
	}

	prio = (argc > optind + 2) ? atoi(argv[optind + 2]) : 0;

	if (mq_send(mqd, argv[optind + 1], strlen(argv[optind + 1]), prio) == -1)
	{
		printf("mq_send error\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
