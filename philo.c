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

void	printf_values(t_waiter *box)
{
	printf("philos: %d\n", box->philos);
    printf("time to die: %d\n", box->time_to_die);
	printf("time to eat: %d\n", box->time_to_eat);
	printf("time to sleep: %d\n", box->time_to_sleep);
	printf("meals: %d\n", box->meals);
}

void	eat()
{
	printf("Philos is thinking\n");
	printf("Philos is eating\n");
}

void	*launch()
{
	
	eat();
	return NULL;
}

int	*fill_array(int *arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		arr[i] = i;
		i++;
	}
	return (arr);
}

void	print_array(int *arr, int len)
{
	int i;

	i = 0;
	while (i < len)
	{	
		printf("#%d value in arr: %d\n", i,arr[i]);
		i++;
	}
}

void	ft_start(t_waiter *box)
{
	int				i;
	uint64_t		time;
	struct timeval	tv;

	i = 0;
	gettimeofday(&tv, NULL);
	// printf("time in sec - %ld\n", tv.tv_sec);
	time = tv.tv_sec * 1000000;
	// printf("time in milisec - %llu\n", time);
	box->phil = (t_philo *)malloc(sizeof(t_philo) * box->philos);
	box->current_time = time;
	while (i < box->philos)
	{
		pthread_create(&box->phil[i].thread, NULL, launch, NULL);
		i++;
	}
	i = 0;
	while (i < box->philos)
	{
		pthread_join(box->phil[i].thread, NULL);
		i++;
	}
	printf("Exit\n");
}

int	main(int argc, char **argv)
{
	t_waiter	*waiter;

    if (argc == 6)
	{
		waiter = (t_waiter *)malloc(sizeof(t_waiter));
        check_errors(argc, argv, waiter);
		ft_start(waiter);
    }
    return (0);
}
