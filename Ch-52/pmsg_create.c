#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
	int flags, opt;
	mode_t perms;
	mqd_t mqd;
	struct mq_attr attr, *attrp;

	attrp = NULL;
	attr.mq_maxmsg = 50;
	attr.mq_msgsize = 2048;
	flags = O_RDWR;

	while ((opt = getopt(argc, argv, "cm:s:x")) != -1)
	{
		switch(opt)
		{
			case 'c':
				flags |= O_CREAT;
				break;

			case 'm':
				attr.mq_maxmsg = atoi(optarg);
				attrp = &attr;
				break;

			case 's':
				attr.mq_msgsize = atoi(optarg);
				attrp = &attr;
				break;

			case 'x':
				flags |= O_EXCL;
				break;

			default:
				usage_error(argv[0]);
		}
	}

	if (optind >= argc)
		usage_error(argv[0]);

	perms = (argc <= optind + 1) ? (S_IRUSR | S_IWUSR) :
		atoi(argv[optind + 1]);

	mqd = mq_open(argv[optind], flags, perms, attrp);
	if (mqd == (mqd_t) - 1)
	{
		printf("open error mq_open\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
