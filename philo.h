#ifndef PHILO_H
# define PHILO_H

# define THINKING 0
# define HUNGRY 1
# define EATING 2

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_philo
{
	int			id;
	pthread_t	thread;
	long		latest_eat;
	long		starving_time;
}	t_philo;

typedef struct	s_waiter
{
    int				philos;
    long 			time_to_die;
	long			time_to_eat;
	long 			time_to_sleep;
	int				meals;
	long			current_time;
	pthread_mutex_t	*mutex;
	t_philo			*phil;
}   t_waiter;

t_waiter	*waiter;

long int	ft_atoi(const char *str);
int			ft_isdigit(int c);
int			ft_ischar(char *str);
void		get_error(t_waiter *box);
void		check_errors(int argc, char **argv, t_waiter *box);
void		ft_start(t_waiter *box);
void		print_array(int *arr, int len);
void		*launch(void *box);
void		*monitor(void *waiter);
void		printf_values(t_waiter *box);
void		init_box(int i, int value, t_waiter *box);
void		mutex_init(t_waiter *waiter);
void		monitor_init(t_waiter *waiter);
void 		phil_init(t_waiter *waiter);
void 		join_init(t_waiter *waiter);

#endif
