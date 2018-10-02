#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_CMD_LEN 200

int system_1(char *command)
{
	int status;
	pid_t child_pid;

	switch(child_pid = fork())
	{
		case -1:
			return -1;

		case 0:
			execl("/bin/sh", "sh", "-c", command, (char *) NULL);
			_exit(127);

		default:
			if(waitpid(child_pid, &status, 0) == -1)
				return -1;
			else
				return status;
	}
}

int main (int argc, char **argv)
{
	char str[MAX_CMD_LEN];
	int status;

	for (;;)
	{
		printf("Command: ");
		fflush(stdout);
		if(fgets(str, MAX_CMD_LEN, stdin) == NULL)
			break;

		status = system_1(str);
		printf("system() returned: status=0x%04x (%d%d)\n",
		       (unsigned int) status, status >> 8, status & 0xff);

		if (status == -1)
		{
			printf("error executing system\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			if(WIFEXITED(status) && WEXITSTATUS(status) == 127)
				printf("(Probably) could not involve shell\n");
			else
				printf("printing wait status\n");
				//printWaitStatus(NULL, status);
		}
	}
	exit (EXIT_SUCCESS);
}


