#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

void	*launch(void *id)
{
	int	*i;

	i = (int *)id;
	printf("Philos #%d is thinking\n", *i);
	printf("Philos #%d is eating\n", *i);
	return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2;

    pthread_create(t1, NULL, launch, NULL);
    return 0;
}
