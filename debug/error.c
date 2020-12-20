#include <stdlib.h>
#include <stdio.h>

void handler(char *errString)
{
	printf("Error: %s.\n", errString);
	printf("program exited.\n");
	exit(-1);
}
