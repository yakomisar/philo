#include "philo.h"

void	init_box(int i, int value, t_waiter *box)
{
	if (i == 1)
		box->philos = value;
	else if (i == 2)
		box->time_to_die = value;
	else if (i == 3)
		box->time_to_eat = value;
	else if (i == 4)
		box->time_to_sleep = value;
	else if (i == 5)
		box->meals = value;
}

void mutex_init(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->philos)
	{
		if (pthread_mutex_init(&(waiter->mutex[i]), NULL) != 0)
			printf("Error: Unable to initialize mutex");
		i++;
	}
}

void	monitor_init(t_waiter *waiter)
{
	int	i;

	i = 0;
	pthread_create(&(waiter->phil[i].thread), NULL, monitor, (void *)waiter);
}

void phil_init(t_waiter *waiter)
{
	int	i;

	i = 1;
	while (i < waiter->philos)
	{
		waiter->phil[i].id = i;
		pthread_create(&(waiter->phil[i].thread), NULL, launch, (void *)&waiter->phil[i]);
		i++;
	}
}

void join_init(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->philos)
	{
		pthread_join(waiter->phil[i].thread, NULL);
		i++;
	}
}
