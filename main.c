#include "philo.h"

long int	ft_atoi(const char *str)
{
	char		*s;
	long int	result;
	int			i;
	short int	sign;

	i = 0;
	sign = 1;
	result = 0;
	s = (char *)str;
	while (s[i] == '\t' || s[i] == '\r' || s[i] == '\n'
		   || s[i] == ' ' || s[i] == '\f' || s[i] == '\v')
		i++;
	if (s[i] == '-' || s[i] == '+')
	{	
		if (s[i] == '-')
			sign *= -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = (result * 10) + (s[i] - '0');
		i++;
	}
	return (result * sign);
}

void	get_error(t_box *box)
{
	write(2, "Error\n", 6);
	free(box);
	exit(0);
}

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	ft_ischar(char *str)
{
	int	a;

	a = 0;
	while (str[a] != '\0')
	{
		if (!ft_isdigit(str[a]) || str[a] != '-' || str[a] != '+')
			return (1);
		a++;
	}
	return (0);
}

void	init_box(int i, int value, t_box *box)
{
	if (i == 1)
		box->philos = value;
	if (i == 2)
		box->time_to_die = value;
	if (i == 3)
		box->time_to_eat = value;
	if (i == 4)
		box->time_to_sleep = value;
}

void	check_errors(int argc, char **argv, t_box *box)
{
	int			i;
	long int	value;

	i = 1;
	while (i < argc)
	{
		value = ft_atoi(argv[i]);
		if (value > 2147483647 || value < -2147483648)
			get_error(box);
		if (value < 0)
			get_error(box);
		else if (!ft_isdigit(*argv[i]))
			get_error(box);
		if (!ft_ischar(argv[i]))
			get_error(box);
		init_box(i, value, box);
		i++;
	}
	if (i == 5)
		init_meals(argv[i], box);
}

int	main(int argc, char **argv)
{
	t_box	*box;

    if	(argc == 6)
	{
		box = (t_box *)malloc(sizeof(t_box));
        check_errors(argc, argv, box);
    }
    return (0);
}


