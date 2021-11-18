#include "philo.h"

void	init_box(int i, int value, t_waiter *box)
{
	if (i == 1)
		box->philos = value;
	else if (i == 2)
		box->time_to_die = value;
	else if (i == 3)
		box->time_to_eat = value * 1000;
	else if (i == 4)
		box->time_to_sleep = value * 1000;
	else if (i == 5)
		box->total_meal = value;
}

void mutex_init(pthread_mutex_t *mutex)
{
	int	i;

	i = 0;
	while (i < waiter->philos)
	{
		if (pthread_mutex_init(&mutex[i], NULL) != 0)
			printf("Error: Unable to initialize mutex");
		i++;
	}
	pthread_mutex_init(&(waiter->text), NULL);
}

void	monitor_init()
{
	pthread_t	my_waiter;

	pthread_create(&my_waiter, NULL, monitor, 0);
	pthread_detach(my_waiter);
}

int	get_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	get_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void phil_even_init(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->philos)
	{
		waiter->phil[i].position = i;
		waiter->phil[i].id = i + 1;
		waiter->phil[i].meal = 0;
		waiter->phil[i].left_fork = get_min(i, (i + 1) % waiter->philos);
		waiter->phil[i].right_fork = get_max(i, (i + 1) % waiter->philos);
		waiter->phil[i].latest_eat = 0;
		waiter->phil[i].state = 0;
		if (waiter->is_meal)
			pthread_create(&(waiter->phil[i].thread), NULL, launch_meal, (void *)&waiter->phil[i]);
		else
			pthread_create(&(waiter->phil[i].thread), NULL, launch, (void *)&waiter->phil[i]);
		i += 2;
	}
	usleep(5000);
}

void phil_odd_init(t_waiter *waiter)
{
	int	i;

	i = 1;
	while (i < waiter->philos)
	{	
		waiter->phil[i].id = i + 1;
		waiter->phil[i].meal = 0;
		waiter->phil[i].left_fork = get_min(i, (i + 1) % waiter->philos);
		waiter->phil[i].right_fork = get_max(i, (i + 1) % waiter->philos);
		waiter->phil[i].latest_eat = 0;
		waiter->phil[i].state = 0;
		if (waiter->is_meal)
			pthread_create(&(waiter->phil[i].thread), NULL, launch_meal, (void *)&waiter->phil[i]);
		else
			pthread_create(&(waiter->phil[i].thread), NULL, launch, (void *)&waiter->phil[i]);
		i += 2;
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
