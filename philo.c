#include "philo.h"

void	init_box(int i, int value, t_box *box)
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

void	printf_values(t_box *box)
{
	printf("philos: %d\n", box->philos);
    printf("time to die: %d\n", box->time_to_die);
	printf("time to eat: %d\n", box->time_to_eat);
	printf("time to sleep: %d\n", box->time_to_sleep);
	printf("meals: %d\n", box->meals);
}

void	eat(int i)
{
	printf("Philos #%d is eating\n", i);
}

void	*launch(void *box)
{
	pthread_mutex_lock(&left_fork);
	pthread_mutex_lock(&right_fork);
	eat(((t_box *)box)->philos);
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

void	ft_start(t_box *box)
{
	pthread_t t1, t2;
	int	*philos;

	philos = (int *)malloc(sizeof(int) * box->philos);
	philos = fill_array(philos, box->philos);
	print_array(philos, box->philos);
	exit(0);
	pthread_create(&t1, NULL, launch, (void *)philos);
	pthread_create(&t2, NULL, launch, (void *)philos);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	//free(philos);
}

int	main(int argc, char **argv)
{
	t_box	*box;

    if (argc == 6)
	{
		box = (t_box *)malloc(sizeof(t_box));
        check_errors(argc, argv, box);
		ft_start(box);
    }
    return (0);
}
