#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>

pthread_mutex_t	left_fork;
pthread_mutex_t	right_fork;

typedef struct	s_box
{
    int	philos;
    int time_to_die;
	int	time_to_eat;
	int time_to_sleep;
	int	meals;
}   t_box;

typedef struct	s_philo
{
	int				i;
	pthread_mutex_t	*left_hand;
	pthread_mutex_t	*right_hand;
}	t_philo;

long int	ft_atoi(const char *str);
void		get_error(t_box *box);
int			ft_isdigit(int c);
int			ft_ischar(char *str);
void		check_errors(int argc, char **argv, t_box *box);
void		ft_start(t_box *box);
void		print_array(int *arr, int len);
int			*fill_array(int *arr, int len);
void		*launch(void *box);
void		eat(int i);
void		printf_values(t_box *box);
void		init_box(int i, int value, t_box *box);

#endif
