#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-cx] [-m maxmsg] [-s msgsize] mq-name "
			"[octal-perms]\n", prog_name);
	fprintf(stderr, "	-c		Create queue (O_CREAT)\n");
	fprintf(stderr, "	-m maxmsg	Set maximum # of messages\n");
	fprintf(stderr, "	-s msgsize	Set maximum message size\n");
	fprintf(stderr, "	-x		Create exclusively (O_EXCL)\n");
	exit (EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	mqd_t mqd;
	struct mq_attr attr;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
	{
		printf("%s mq-name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1], O_RDONLY);
	if (mqd == (mqd_t) - 1)
	{
		printf("open error mq_open\n");
		exit(EXIT_FAILURE);
	}

	if (mq_getattr(mqd, &attr) == -1)
	{
		printf("mq_getattr error\n");
		exit(EXIT_FAILURE);
	}

	printf("Maximum # of messages on queue:		%ld\n", attr.mq_maxmsg);
	printf("Maximum message size:			%ld\n", attr.mq_msgsize);
	printf("# of messages currently on queue:	%ld\n", attr.mq_curmsgs);
	exit(EXIT_SUCCESS);
}
