#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void atexitfunc_1(void)
{
	printf("atexit function 1 called\n");
}

static void atexitfunc_2(void)
{
	printf("atexit function 2 called\n");
}

static void onexitfunc(int exit_status, void *arg)
{
	printf("on_exit function called: status=%d, arg=%ld\n",
	       exit_status, (long)arg);
}

int main(int argc, char **argv)
{
	if (on_exit(onexitfunc, (void *)10) != 0)
	{
		printf("failed on_exit registration\n");
		exit (1);
	}

	if (atexit(atexitfunc_1) != 0)
	{
		printf("failed atexit registration\n");
		exit(1);
	}

	if (atexit(atexitfunc_2) != 0)
	{
		printf("failed atexit registration\n");
		exit(2);
	}

	if (on_exit(onexitfunc, (void *)20) != 0)
	{
		printf("failed on_exit registration\n");
		exit(2);
	}

	exit(2);
}
