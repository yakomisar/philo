#include "philo.h"

void	printf_values(t_waiter *box)
{
	printf("philos: %d\n", box->philos);
    printf("time to die: %ld\n", box->time_to_die);
	printf("time to eat: %ld\n", box->time_to_eat);
	printf("time to sleep: %ld\n", box->time_to_sleep);
	printf("meals: %d\n", box->meals);
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

long	get_time()
{
	long			result;
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	result = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (result);
}

void	*monitor(void *waiter)
{
	t_waiter	*monitor;
	
	monitor = (t_waiter *)waiter;
	monitor->current_time = get_time();
	printf("Monitor time #%ld\n", monitor->current_time);
	return (NULL);
}

void	*launch(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	philosopher->latest_eat = get_time();
	while (1)
	{
		mutex();
		mutex();
		eat(waiter->time_to_eat);
		mutex();
		mutex();
		sleeping();
		thinking();
	}
	return (NULL);
}

void	ft_start(t_waiter *waiter)
{
	waiter->phil = (t_philo *)malloc(sizeof(t_philo) * waiter->philos + 1);
	waiter->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * waiter->philos);
	monitor_init(waiter);
	phil_init(waiter);
	mutex_init(waiter);
	join_init(waiter);
	printf("Exit\n");
}

int	main(int argc, char **argv)
{
	if (argc == 5)
	{
		waiter = (t_waiter *)malloc(sizeof(t_waiter));
		check_errors(argc, argv, waiter);
		ft_start(waiter);
	}
    if (argc == 6)
	{
		waiter = (t_waiter *)malloc(sizeof(t_waiter));
        check_errors(argc, argv, waiter);
		ft_start(waiter);
    }
	if (waiter)
		free(waiter);
    return (0);
}
