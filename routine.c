/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 09:30:24 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/07 17:00:55 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *truc)
{
	t_philo *philo;
	int count;
	
	philo = truc;
	count = 0;
	if (!(philo->id % 2 == 0))
		ft_usleep(philo->info->time_to_eat * 0.25);
	while_routine(philo, count);
	return(0);
}
int while_routine(t_philo *philo, int count)
{
	while (!is_dead(philo->info))
	{
		if (philo->info->nb_philo % 2 && count)
			ft_usleep(philo->info->time_to_die * 0.25);
		if (philo->id % 2 != 0)
		{
			if(case1(philo))
				break;
		}
		else
			case2(philo);
		aftercase(philo);
		count++;
		if (count == philo->info->max_eat)
		{
			pthread_mutex_lock(&philo->info->finish);
			philo->info->finished++;
			pthread_mutex_unlock(&philo->info->finish);
			return(0);
		}
		print(philo, "is sleeping\n");
		ft_usleep(philo->info->time_to_sleep);
		print(philo, "is thinking\n");
	}
	return(0);
}
int case1(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_right);
	print(philo, "has taken a fork\n");
	if(philo->fork_right == &philo->fork)
	{
		pthread_mutex_unlock(philo->fork_right);
		return(1);//break
	}
	pthread_mutex_lock(philo->fork_left);
	print(philo, "has taken a fork\n");
	return(0);//nothing just the next
}
void case2(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	print(philo, "has taken a fork\n");
	pthread_mutex_lock(philo->fork_right);
	print(philo, "has taken a fork\n");
}
void aftercase(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = gettime();
	pthread_mutex_unlock(&philo->eating);
	print(philo, "is eating\n");		
	ft_usleep(philo->info->time_to_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}


