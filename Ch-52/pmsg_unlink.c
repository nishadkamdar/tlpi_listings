#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>

int main(int argc, char *argv[])
{
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
	{
		printf("%s mq-name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (mq_unlink(argv[1]) == -1)
	{
		printf("mq_unlink error\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
