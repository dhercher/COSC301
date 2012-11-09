#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char **argv) 
{
	DIR *curr;
	char *dir;
	int *pl;
	dir = getcwd(dir, pl);
	printf("%s\n", dir);
	
	curr = opendir(".");
	if(curr == NULL)
	{
		perror("Error in creating sample.dat");
		return EXIT_FAILURE;
	}
	struct dirent *cdir;
	long *a;
	struct stat statbuf;
	while((cdir = readdir(curr)) != NULL)
	{
		if (stat(cdir->d_name, &statbuf) == -1)
			continue;
		printf("%s    ", cdir->d_name);
		
		printf("%d", (int)statbuf.st_size);
		// printf(" %9jd", (intmax_t)statbuf.st_size);
		printf("\n");
	}
}