#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_philo
{
	int				id;
	int				meal;
	int				left_fork;
	int				right_fork;
	int				state;
	long			latest_eat;	
	struct s_waiter	*waiter;
	pthread_t		thread;
}	t_philo;

typedef struct	s_waiter
{
    int				philos;
    long 			time_to_die;
	long			time_to_eat;
	long 			time_to_sleep;
	int				is_meal;
	int				total_meal;
	int				consumed;
	long			start_time;
	int				is_died;
	int				(*answer)(int a, t_philo *ph);
	pthread_mutex_t	text;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*fork;
	t_philo			*phil;
}   t_waiter;

long int	ft_atoi(const char *str);
long		get_time();
int			ft_isdigit(int c);
int			ft_ischar(char *str);
void		get_error(t_waiter *box);
void		check_errors(int argc, char **argv, t_waiter *box);
void		ft_start(t_waiter *box);
void		print_array(int *arr, int len);
void		*launch(void *box);
void		*launch_meal(void *phil);
void		*monitor(void *value);
void		init_box(int i, int value, t_waiter *box);
void		mutex_init(t_waiter *waiter);
void		monitor_init(t_waiter *waiter);
void		phil_even_init(t_waiter *waiter);
void		phil_odd_init(t_waiter *waiter);
void 		join_init(t_waiter *waiter);

#endif
