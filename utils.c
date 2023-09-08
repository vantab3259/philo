/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 05:41:10 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/07 09:12:41 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_info	*info)
{
	int	dead;
	
	pthread_mutex_lock(&info->death);
	dead = info->dead;
	pthread_mutex_unlock(&info->death);
	return (dead); 
}

long	gettime(void)
{
	struct timeval time;
	
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->info->print);
	if (is_dead(philo->info) == 1)
	{
		pthread_mutex_unlock(&philo->info->print);
		return ;
	}
	printf("%ld %d %s", gettime() - philo->info->time_start, philo->id + 1, str);
	pthread_mutex_unlock(&philo->info->print);
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
