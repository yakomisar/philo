#include "philo.h"

long	get_time()
{
	long			result;
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	result = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (result);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
	}
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr_long(long n)
{
	if (n < 0)
	{
		n = n * -1;
		ft_putchar('-');
	}
	if (n < 10)
	{
		ft_putchar(n + '0');
		return ;
	}
	ft_putnbr_long(n / 10);
	ft_putchar((n % 10) + '0');
	return ;
}

void	ft_putnbr(int n)
{
	if (n < 0)
	{
		n = n * -1;
		ft_putchar('-');
	}
	if (n < 10)
	{
		ft_putchar(n + '0');
		return ;
	}
	ft_putnbr(n / 10);
	ft_putchar((n % 10) + '0');
	return ;
}

void	*monitor()
{
	int			i;
	long		starving;
	
	while (1)
	{
		i = 0;
		while (i < waiter->philos)
		{
			if (waiter->is_meal && waiter->consumed == (waiter->total_meal * waiter->philos))
				exit(1);
			starving = get_time() - waiter->phil[i].latest_eat;
			if	((starving > waiter->time_to_die) && waiter->phil[i].latest_eat != 0)
			{
				if (waiter->is_meal && waiter->consumed == (waiter->total_meal * waiter->philos))
					exit(1);
				pthread_mutex_lock(&(waiter->text));
				printf("%ld %d died\n", get_time() - waiter->start_time, waiter->phil[i].id);
				pthread_mutex_unlock(&(waiter->text));
				exit(1);
			}
			i++;
		}
	}
	return (NULL);
}

void	my_usleep(long value)
{
	long	check;
	long	stop;

	check = get_time();
	stop = value / 1000;
	while ((get_time() - check) < stop)
		usleep(1000);
}

void	eating(t_philo *philosopher, long time)
{
	philosopher->latest_eat = get_time();
	philosopher->state = 1;
	pthread_mutex_lock(&(waiter->text));
	printf("%ld %d is eating\n", get_time() - waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(waiter->text));
	my_usleep(time);
	philosopher->meal++;
	waiter->consumed++;
}

void	sleeping(t_philo *philosopher, long time)
{
	philosopher->state = 0;
	pthread_mutex_lock(&(waiter->text));
	printf("%ld %d is sleeping\n", get_time() - waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(waiter->text));
	my_usleep(time);
}

int	ask_waiter(int n)
{
	int		left;
	int		right;
	long	left_last_meal;
	long	right_last_meal;

	left = (n - 1 + waiter->philos) % waiter->philos;
	right = (n + 1) % waiter->philos;
	// printf("left is %d\n", left);
	// printf("middle is %d\n", n);
	// printf("right is %d\n", right);
	left_last_meal = waiter->phil[left].latest_eat;
	// printf("ph#%d last meal at %ld for neighbor of %d\n", left, left_last_meal, n);
	right_last_meal = waiter->phil[right].latest_eat;
	// printf("ph#%d last meal is %ld\n", n+1, waiter->phil[n].latest_eat);
	// printf("ph#%d last meal at %ld for neighbor of %d\n",right, right_last_meal, n);
	if ((waiter->phil[n].latest_eat <= left_last_meal) && (waiter->phil[n].latest_eat <= right_last_meal))
	{
		// printf("%d can eat\n", waiter->phil[n].id);
		return (1);
	}
	// printf("%d CANNOT eat\n", waiter->phil[n].id);
	return (0);
}

void	*launch(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	while (1)
	{
		pthread_mutex_lock(&mutex[philosopher->left_fork]);
		printf("%ld %d has taken a fork\n", get_time() - waiter->start_time, philosopher->id);
		pthread_mutex_lock(&mutex[philosopher->right_fork]);
		printf("%ld %d has taken a fork\n", get_time() - waiter->start_time, philosopher->id);
		// printf("%d - position is checking\n", philosopher->id);
		if (waiter->answer(philosopher->id - 1))
		{
			eating(philosopher, waiter->time_to_eat);
		}
		pthread_mutex_unlock(&mutex[philosopher->right_fork]);
		pthread_mutex_unlock(&mutex[philosopher->left_fork]);
		if (philosopher->state == 1)
		{
			sleeping(philosopher, waiter->time_to_sleep);
			printf("%ld %d is thinking\n", get_time() - waiter->start_time, philosopher->id);
		}
		my_usleep(1000);
	}
	return (NULL);
}

void	*launch_meal(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	while (1)
	{
		if (philosopher->meal < waiter->total_meal)
		{
			pthread_mutex_lock(&mutex[philosopher->left_fork]);
			pthread_mutex_lock(&(waiter->text));
			printf("%ld %d has taken a fork\n", get_time() - waiter->start_time, philosopher->id);
			pthread_mutex_unlock(&(waiter->text));
			pthread_mutex_lock(&mutex[philosopher->right_fork]);
			pthread_mutex_lock(&(waiter->text));
			printf("%ld %d has taken a fork\n", get_time() - waiter->start_time, philosopher->id);
			pthread_mutex_unlock(&(waiter->text));
			eating(philosopher, waiter->time_to_eat);
			pthread_mutex_unlock(&mutex[philosopher->right_fork]);
			pthread_mutex_unlock(&mutex[philosopher->left_fork]);
			sleeping(philosopher, waiter->time_to_sleep);
			printf("%ld %d is thinking\n", get_time() - waiter->start_time, philosopher->id);
		}
	}
	return (NULL);
}

void	ft_start(t_waiter *waiter)
{
	waiter->phil = (t_philo *)malloc(sizeof(t_philo) * waiter->philos);
	mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * waiter->philos);
	mutex_init(mutex);
	waiter->start_time = get_time();
	phil_even_init(waiter);
	phil_odd_init(waiter);
	monitor_init();
	join_init(waiter);
}

void	check_one()
{
	usleep(waiter->time_to_die * 1000);
	ft_putnbr_long(get_time() - waiter->start_time);
	write(1, " 1 died\n", 8);
	if (waiter)
		free(waiter);
	exit(1);
}

int	main(int argc, char **argv)
{
	waiter = (t_waiter *)malloc(sizeof(t_waiter));
	waiter->start_time = get_time();
	waiter->is_died = 0;
	waiter->answer = ask_waiter;
	if (argc == 5)
	{
		check_errors(argc, argv, waiter);
		if (waiter->philos == 1)
			check_one();
		waiter->is_meal = 0;
		ft_start(waiter);
	}
    if (argc == 6)
	{
        check_errors(argc, argv, waiter);
		if (waiter->philos == 1)
			check_one();
		waiter->is_meal = 1;
		waiter->consumed = 0;
		ft_start(waiter);
    }
	if (waiter)
		free(waiter);
    return (0);
}
