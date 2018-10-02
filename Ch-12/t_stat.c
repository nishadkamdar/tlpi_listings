#define _DEFAULT_SOURCE
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static void displayStatInfo(const struct stat *sb)
{
	printf("File type:		");

	switch(sb->st_mode & S_IFMT)
	{
		case S_IFREG:	printf("regular file\n");	break;
		case S_IFDIR:	printf("directory\n");		break;
		case S_IFCHR:	printf("character device\n");	break;
		case S_IFBLK:	printf("block device\n");	break;
		case S_IFLNK:	printf("symbolic (soft) link");	break;
		case S_IFIFO:	printf("FIFO or pipe\n");	break;
		case S_IFSOCK:	printf("socket\n");		break;
		default:	printf("unknown file type?\n");	break;
	}

	printf("Device containing i-node: major=%ld   minor=%ld\n",
			(long)major(sb->st_dev), (long)minor(sb->st_dev));

	printf("I-node number:		%ld\n", (long) sb->st_ino);

	printf("Mode:			%lo\n",
			(unsigned long) sb->st_mode);

	if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
		printf("	special bits set:	%s%s%s\n",
				(sb->st_mode & S_ISUID) ? "set-UID " : "",
				(sb->st_mode & S_ISGID) ? "set-GID"  : "",
				(sb->st_mode & S_ISVTX) ? "sticky"   : "");
				
	printf ("Number of (hard) links:  %ld\n", (long) sb->st_nlink);

	printf("Ownership:		UID=%ld	GID=%ld\n",
			(long) sb->st_uid, (long) sb->st_gid);

	if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
		printf("Device number (st_dev): major=%ld; minor=%ld\n",
				(long) major(sb->st_dev), (long)minor(sb->st_dev));

}

int main(int argc, char *argv[])
{
	struct stat sb;
	char statLink;
	int fname;

	statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;

	fname = statLink ? 2: 1;

	if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
	{
		printf("%s [-l] file\n"
		       "	-l = use lstat() instead of stat()\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (statLink) {
		if (lstat(argv[fname], &sb) == -1)
		{
			printf("lsat failed\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			if (stat(argv[fname], &sb) == -1)
			{
				printf("stat failed\n");
				exit(EXIT_FAILURE);
			}
		}

		displayStatInfo(&sb);

		exit(EXIT_SUCCESS);
	}
}
