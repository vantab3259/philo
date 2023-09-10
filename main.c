/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 02:37:01 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/10 14:13:44 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_info *info)
{
	int	dead;

	pthread_mutex_lock(&info->death);
	dead = info->dead;
	pthread_mutex_unlock(&info->death);
	return (dead);
}

int	death_or_no(t_info *info, t_philo *philo, long eating)
{
	if (eating > info->time_to_die)
	{
		pthread_mutex_lock(&info->death);
		info->dead = 1;
		pthread_mutex_unlock(&info->death);
		pthread_mutex_lock(&info->print);
		printf("%ld %d %s", gettime() - philo->info->time_start, philo->id + 1,
			"died\n");
		pthread_mutex_unlock(&info->print);
		return (1);
	}
	return (0);
}

void	check_death(t_info *info, t_philo *philo, int i)
{
	int		finish;
	long	eating;

	finish = 0;
	while (1)
	{
		i = -1;
		while (++i < info->nb_philo)
		{
			pthread_mutex_lock(&philo[i].eating);
			eating = gettime() - philo[i].last_meal;
			pthread_mutex_unlock(&philo[i].eating);
			if (death_or_no(info, philo, eating))
				return ;
			pthread_mutex_lock(&philo->info->finish);
			finish = info->finished;
			pthread_mutex_unlock(&philo->info->finish);
			if (finish == info->nb_philo)
			{
				print(philo, "has finish to eat\n");
				return ;
			}
		}
		usleep(100);
	}
}

int	main(int ac, char **av)
{
	t_info	info;
	int		i;
	t_philo	*philo;

	i = 0;
	philo = 0;
	if (!(ac >= 5 && ac <= 6))
		return (write(2, "nombre d'arguments\n", 19), 2);
	philo = init_philo(&info, philo, av, ac);
	start_philo(&info, philo);
	check_death(&info, philo, i);
	while (i < info.nb_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	pthread_mutex_destroy(&info.print);
	pthread_mutex_destroy(&info.finish);
	pthread_mutex_destroy(&info.death);
	free(philo);
}
