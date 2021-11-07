#include "philo.h"

void	printf_values(t_waiter *box)
{
	printf("philos: %d\n", box->philos);
    printf("time to die: %ld\n", box->time_to_die);
	printf("time to eat: %ld\n", box->time_to_eat);
	printf("time to sleep: %ld\n", box->time_to_sleep);
	printf("meals: %d\n", box->total_meal);
}

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
			starving = get_time() - waiter->phil[i].latest_eat;
			if	((starving > waiter->time_to_die) && waiter->phil[i].latest_eat != 0)
			{
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

void	eating(t_philo *philosopher, long time)
{
	pthread_mutex_lock(&(waiter->text));
	printf("%ld %d is eating\n", get_time() - waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(waiter->text));
	//philosopher->latest_eat = get_time();
	usleep(time);
	philosopher->latest_eat = get_time();
	philosopher->meal++;
}

void	sleeping(t_philo *philosopher, long time)
{
	pthread_mutex_lock(&(waiter->text));
	printf("%ld %d is sleeping\n", get_time() - waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(waiter->text));
	usleep(time);
}

void	thinking(t_philo *philosopher)
{
	pthread_mutex_lock(&(waiter->text));
	printf("%ld %d is thinking\n", get_time() - waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(waiter->text));
	usleep(1000);
}

void	*launch(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	while (1)
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
		thinking(philosopher);
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
