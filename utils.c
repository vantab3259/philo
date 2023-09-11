/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 05:41:10 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/10 14:47:40 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_atoi(int *result, char *str)
{
	int	sign;

	sign = 1;
	while ((*str > 8 && *str < 14) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (false);
	*result = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (*result > *result * 10 - 48 + *str && !(*result == INT_MAX / 10
				&& *str == '8' && !str[1] && sign < 0))
			return (false);
		*result = *result * 10 - 48 + *str;
		str++;
	}
	if (*str)
		return (false);
	return (*result *= sign, true);
}

long	gettime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->info->print);
	if (is_dead(philo->info) == 1)
	{
		pthread_mutex_unlock(&philo->info->print);
		return (1);
	}
	printf("%ld %d %s", gettime() - philo->info->time_start, philo->id + 1,
		str);
	pthread_mutex_unlock(&philo->info->print);
	return (0);
}

void	ft_usleep(long ms)
{
	long	begin;

	begin = gettime();
	while (gettime() - begin < ms)
	{
		usleep(10);
	}
}

int	ft_sleep(t_philo *philo)
{
	if (print(philo, "is sleeping\n"))
		return (EXIT_SUCCESS);
	ft_usleep(philo->info->time_to_sleep);
	return (EXIT_SUCCESS);
}
