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

pthread_mutex_t	mutex;

typedef struct	s_philo
{
	int			id;
	pthread_t	thread;
}	t_philo;

typedef struct	s_waiter
{
    int			philos;
    int 		time_to_die;
	int			time_to_eat;
	int 		time_to_sleep;
	int			meals;
	uint64_t	current_time;
	t_philo		*phil;
}   t_waiter;

long int	ft_atoi(const char *str);
int			ft_isdigit(int c);
int			ft_ischar(char *str);
int			*fill_array(int *arr, int len);
void		get_error(t_waiter *box);
void		check_errors(int argc, char **argv, t_waiter *box);
void		ft_start(t_waiter *box);
void		print_array(int *arr, int len);
void		*launch();
void		eat();
void		printf_values(t_waiter *box);
void		init_box(int i, int value, t_waiter *box);

#endif
