#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>

typedef struct	s_box
{
    int	philos;
    int time_to_die;
	int	time_to_eat;
	int time_to_sleep;
	int	meals_count[philos];
}   t_box;

#endif
