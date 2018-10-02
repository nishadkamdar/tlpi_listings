#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int istack = 222;

	switch (vfork()) {
	  case -1:
		printf("error running vfork\n");
		exit(EXIT_FAILURE);

	  case 0:
		sleep(3);
		write(STDOUT_FILENO, "Child executing\n", 16);
		istack *= 3;
		exit(EXIT_SUCCESS);
	
	  default:
		write(STDOUT_FILENO, "Parent executing\n", 17);
		printf("istack=%d\n", istack);
		exit(EXIT_SUCCESS);
	}
}
