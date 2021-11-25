#include "philo.h"

void	stop_and_exit(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->philos)
	{
		pthread_mutex_destroy(&(waiter->fork[i]));	
		i++;
	}
	pthread_mutex_destroy(&(waiter->text));
	free(waiter->phil);
	free(waiter->mutex);
}

long	get_time()
{
	long			result;
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	result = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (result);
}

void	kill(t_philo *ph)
{
	pthread_mutex_lock(&(ph->waiter->text));
	ph->waiter->is_died = 1;
	printf("%ld %d died\n", get_time() - ph->waiter->start_time, ph->id);
	pthread_mutex_unlock(&(ph->waiter->text));
}

void	stop_simulation(t_philo *ph)
{
	pthread_mutex_lock(&(ph->waiter->text));
	ph->waiter->is_died = 1;
	pthread_mutex_unlock(&(ph->waiter->text));
}

void	*monitor(void *value)
{
	int			i;
	long		starving;
	t_waiter	*waiter;

	waiter = (t_waiter *)value;
	while (1)
	{
		i = 0;
		while (i < waiter->philos)
		{
			if (waiter->is_meal && waiter->consumed == (waiter->total_meal * waiter->philos))
			{
				stop_simulation(&(waiter->phil[i]));
				return (NULL);
			}
			starving = get_time() - waiter->phil[i].latest_eat;
			if	((starving > waiter->time_to_die) && waiter->phil[i].latest_eat != 0)
			{
				kill(&(waiter->phil[i]));
				return (NULL);
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
	pthread_mutex_lock(&(philosopher->waiter->text));
	if(!(philosopher->waiter->is_died))
		printf("%ld %d is eating\n", get_time() - philosopher->waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(philosopher->waiter->text));
	my_usleep(time);
	philosopher->meal++;
	philosopher->waiter->consumed++;
}

void	sleeping(t_philo *philosopher, long time)
{
	philosopher->state = 0;
	pthread_mutex_lock(&(philosopher->waiter->text));
	if(!(philosopher->waiter->is_died))
		printf("%ld %d is sleeping\n", get_time() - philosopher->waiter->start_time, philosopher->id);
	pthread_mutex_unlock(&(philosopher->waiter->text));
	my_usleep(time);
}

int	ask_waiter(int n, t_philo *ph)
{
	int		left;
	int		right;
	long	left_last_meal;
	long	right_last_meal;

	left = (n - 1 + ph->waiter->philos) % ph->waiter->philos;
	right = (n + 1) % ph->waiter->philos;
	left_last_meal = ph->waiter->phil[left].latest_eat;
	right_last_meal = ph->waiter->phil[right].latest_eat;
	if ((ph->waiter->phil[n].latest_eat <= left_last_meal) && (ph->waiter->phil[n].latest_eat <= right_last_meal))
		return (1);
	return (0);
}

void	take_forks(t_philo *philosopher)
{
	pthread_mutex_lock(&philosopher->waiter->fork[philosopher->left_fork]);
	if (!(philosopher->waiter->is_died))
		printf("%ld %d has taken a fork\n", get_time() - philosopher->waiter->start_time, philosopher->id);
	pthread_mutex_lock(&philosopher->waiter->fork[philosopher->right_fork]);
	if (!(philosopher->waiter->is_died))
		printf("%ld %d has taken a fork\n", get_time() - philosopher->waiter->start_time, philosopher->id);
}

void	put_forks(t_philo *philosopher)
{
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->right_fork]);
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->left_fork]);
}

void	*launch(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	while (!(philosopher->waiter->is_died))
	{
		take_forks(philosopher);
		if (philosopher->waiter->is_died)
			break ;
		if (philosopher->waiter->answer(philosopher->id - 1, philosopher))
			eating(philosopher, philosopher->waiter->time_to_eat);
		put_forks(philosopher);
		if (philosopher->state == 1)
		{
			sleeping(philosopher, philosopher->waiter->time_to_sleep);
			if(!(philosopher->waiter->is_died))
				printf("%ld %d is thinking\n", get_time() - philosopher->waiter->start_time, philosopher->id);
		}
		my_usleep(1000);
	}
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->right_fork]);
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->left_fork]);
	pthread_mutex_unlock(&(philosopher->waiter->text));
	return (NULL);
}

void	*launch_meal(void *phil)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)phil;
	while (!(philosopher->waiter->is_died))
	{
		if (philosopher->meal < philosopher->waiter->total_meal)
		{
			take_forks(philosopher);
			if (philosopher->waiter->is_died)
				break ;
			if (philosopher->waiter->answer(philosopher->id - 1, philosopher))
				eating(philosopher, philosopher->waiter->time_to_eat);
			put_forks(philosopher);
			if (philosopher->state == 1)
			{
				sleeping(philosopher, philosopher->waiter->time_to_sleep);
				if (!(philosopher->waiter->is_died))
					printf("%ld %d is thinking\n", get_time() - philosopher->waiter->start_time, philosopher->id);
			}
			my_usleep(1000);
		}
	}
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->right_fork]);
	pthread_mutex_unlock(&philosopher->waiter->fork[philosopher->left_fork]);
	pthread_mutex_unlock(&(philosopher->waiter->text));
	return (NULL);
}

void	ft_start(t_waiter *waiter)
{
	waiter->phil = (t_philo *)malloc(sizeof(t_philo) * waiter->philos);
	waiter->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * waiter->philos);
	mutex_init(waiter);
	waiter->start_time = get_time();
	phil_even_init(waiter);
	phil_odd_init(waiter);
	monitor_init(waiter);
	join_init(waiter);
}

void	check_one(t_waiter *waiter)
{
	my_usleep(waiter->time_to_die * 1000);
	printf("%ld 1 died\n", get_time() - waiter->start_time);
}

int	main(int argc, char **argv)
{
	t_waiter	*waiter;
	waiter = (t_waiter *)malloc(sizeof(t_waiter));
	waiter->start_time = get_time();
	waiter->is_died = 0;
	waiter->answer = ask_waiter;
	if (argc == 5)
	{
		check_errors(argc, argv, waiter);
		if (waiter->philos == 1)
			check_one(waiter);
		waiter->is_meal = 0;
		ft_start(waiter);
		stop_and_exit(waiter);
	}
    if (argc == 6)
	{
        check_errors(argc, argv, waiter);
		if (waiter->philos == 1)
			check_one(waiter);
		waiter->is_meal = 1;
		waiter->consumed = 0;
		ft_start(waiter);
		stop_and_exit(waiter);
    }
	free(waiter);
    return (0);
}
