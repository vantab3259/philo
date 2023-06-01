#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
void	*routine(void *a)
{
	int*	b = a;
	printf("threads en marche %d\n", b[0]);
	sleep(3);
	printf("on l'a fait simultanement\n");
}

int	main(int argc, char *argv[])
{
	int a[] = {10, 2};
	pthread_t t1;
	pthread_create(&t1, NULL, &routine, (void*)a);
	pthread_join(t1, NULL);
	return (0);
}

