#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *arg_vec[10];
	char *env_vec[] = {"GREET=salut", "BYE=adieu", NULL};

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
	{
		printf("%s pathname \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	arg_vec[0] = strrchr(argv[1], '/');
	if (arg_vec[0] != NULL)
		arg_vec[0]++;
	else
		arg_vec[0] = argv[1];
	arg_vec[1] = "hello world";
	arg_vec[2] = "goodbye";
	arg_vec[3] = NULL;
	
	execve(argv[1], arg_vec, env_vec);
	printf("error runing execve\n");
	exit(EXIT_FAILURE);
}	
