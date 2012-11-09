#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <string.h>


int main(int argc, char **argv) 
{
	int nf;
	printf("%s\n", argv[1]);
	nf = creat(argv[1], S_IREAD|S_IWRITE);
	if(nf == -1)
	{
		perror("Error in creating sample.dat");
		return EXIT_FAILURE;
	}
	// printf("%s\n", 'File created');
	nf = close(nf);
	if(nf == -1)
	{
		perror("Error in creating sample.dat");
		return EXIT_FAILURE;
	}
	return 0;
}