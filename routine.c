/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:21:23 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/10 14:12:03 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_unlock(t_philo *philo, int opt)
{
	if (opt == 0)
		pthread_mutex_unlock(philo->fork_right);
	else if (opt == 1)
		pthread_mutex_unlock(philo->fork_left);
	else
	{
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
	}
}

static int	take_fork(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->fork_left);
		if (print(philo, "has taken a fork\n"))
			return (ft_unlock(philo, 1), 1);
		if (philo->fork_left == philo->fork_right)
			return (ft_unlock(philo, 1), 1);
		pthread_mutex_lock(philo->fork_right);
		if (print(philo, "has taken a fork\n"))
			return (ft_unlock(philo, 2), 1);
	}
	else
	{
		pthread_mutex_lock(philo->fork_right);
		if (print(philo, "has taken a fork\n"))
			return (ft_unlock(philo, 0), 1);
		pthread_mutex_lock(philo->fork_left);
		if (print(philo, "has taken a fork\n"))
			return (ft_unlock(philo, 2), 1);
	}
	return (0);
}

int	is_eating(t_philo *philo)
{
	if (take_fork(philo))
		return (1);
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = gettime();
	pthread_mutex_unlock(&philo->eating);
	if (print(philo, "is eating\n"))
		return (ft_unlock(philo, 2), 1);
	ft_usleep(philo->info->time_to_eat);
	philo->count_meal++;
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	if (philo->count_meal == philo->info->max_eat)
	{
		pthread_mutex_lock(&philo->info->finish);
		philo->info->finished++;
		pthread_mutex_unlock(&philo->info->finish);
		return (1);
	}
	return (0);
}

void	*routine(void *truc)
{
	t_philo	*philo;
	int		count;

	count = 0;
	philo = truc;
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat * 0.25);
	while (1)
	{
		if (philo->info->nb_philo % 2 && philo->count_meal)
			ft_usleep(philo->info->time_to_die * 0.25);
		if (is_eating(philo))
			break ;
		if (ft_sleep(philo))
			break ;
		if (print(philo, "is thinking\n"))
			break ;
	}
	return (0);
}
