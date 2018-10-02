#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int idata = 111;

int main(void)
{
	int istack = 222;
	pid_t child_pid;

	switch(child_pid = fork()) {
	  case -1:
		printf("error running fork\n");
		exit(EXIT_FAILURE);
	  
	  case 0:
		idata *= 3;
		istack *= 3;
		break;

	  default:
		sleep(3);
		break;
	}

	printf("PID:%ld %s idata=%d istack=%d\n", (long) getpid(),
	       (child_pid == 0) ? "(child) " : "(parent)", idata, istack);

	exit(EXIT_SUCCESS);
}
