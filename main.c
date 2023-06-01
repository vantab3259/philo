/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 02:37:01 by mudoh             #+#    #+#             */
/*   Updated: 2023/05/18 03:02:21 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philo(t_info *info, t_philo *philo, char **av, int ac)
{
	info->nb_philo = atoi(av[1]);
	info->time_to_die = atoi(av[2]);
	info->time_to_eat = atoi(av[3]);
	info->time_to_sleep  = atoi(av[4]);
	if (ac == 6)
		info->tmax_to_eat = atoi(av[5]);
	else 
		info->tmax_to_eat = 0;
	info->dead  = 0;
	info->finished  = 0;
	philo = calloc(sizeof(t_philo), info->nb_philo);
	info->time_start = gettime();
	pthread_mutex_init(&info->print, NULL);
	pthread_mutex_init(&info->finish, NULL);
	pthread_mutex_init(&info->death, NULL);
	for (int i = 0; i < info->nb_philo; i++)
	{
		philo[i].info = info;
		philo[i].id = i;
		philo[i].last_meal = info->time_start;
		pthread_mutex_init(&philo[i].fork, NULL);
		pthread_mutex_init(&philo[i].eating, NULL);
		philo[i].fork_left = &philo[i].fork;
		if (i == info->nb_philo - 1)
			philo[i].fork_right = &philo[0].fork;
		else
			philo[i].fork_right = &philo[i+1].fork;
	}
	return (philo);
}

void	*routine(void *truc)
{
	t_philo *philo;
	philo = truc;
	int count;
	
	count = 0;
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat * 0.25);
	while (!is_dead(philo->info))
	{
		if (philo->info->nb_philo % 2 && count)
			ft_usleep(philo->info->time_to_die * 0.25);
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->fork_right);
			print(philo, "has taken a fork\n");
			if(philo->fork_right == &philo->fork)
			{
				pthread_mutex_unlock(philo->fork_right);
				break;
			}
			pthread_mutex_lock(philo->fork_left);
			print(philo, "has taken a fork\n");
		}
		else
		{
			pthread_mutex_lock(philo->fork_left);
			print(philo, "has taken a fork\n");
			pthread_mutex_lock(philo->fork_right);
			print(philo, "has taken a fork\n");
		}
		pthread_mutex_lock(&philo->eating);
		philo->last_meal = gettime();
		pthread_mutex_unlock(&philo->eating);
		print(philo, "is eating\n");		
		ft_usleep(philo->info->time_to_eat);
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		count++;
		if (count == philo->info->tmax_to_eat)
		{
			pthread_mutex_lock(&philo->info->finish);
			philo->info->finished++;
			pthread_mutex_unlock(&philo->info->finish);
			return (0);
		}
		print(philo, "is sleeping\n");
		ft_usleep(philo->info->time_to_sleep);
		print(philo, "is thinking\n");
	}
	return(0);
}

int start_philo(t_info *info, t_philo *philo)
{
	int i = 0;
	
	while (i < info->nb_philo)
	{
		pthread_create(&philo[i].thread, NULL, &routine, &philo[i]);
		i++;
	}
	return(0);
}

void	check_death(t_info *info, t_philo *philo)
{
	int finish;
	long eating;
	
	finish = 0;
	while (1)
	{
		int	i = 0;
		while (i < info->nb_philo)
		{
			pthread_mutex_lock(&philo[i].eating);
			eating = gettime() - philo[i].last_meal;
			pthread_mutex_unlock(&philo[i].eating);
			if (eating > info->time_to_die)
			{
				pthread_mutex_lock(&info->death);
				info->dead = 1;
				pthread_mutex_unlock(&info->death);
				pthread_mutex_lock(&info->print);
				printf("%08ld %d %s", gettime() - philo->info->time_start, philo->id, "has died\n");
				pthread_mutex_unlock(&info->print);
				return ;
			}
			pthread_mutex_lock(&philo->info->finish);
			finish = info->finished;
			pthread_mutex_unlock(&philo->info->finish);
			if(finish == info->nb_philo)
			{
				print(philo, "has finish to eat\n");
				return ;
			}
			i++;
		}
		usleep(100);
	}
}
	
int	main(int ac, char **av)
{
	t_info	info;
	int		i;
	t_philo *philo;

	i = 0;
	philo = 0;
	if (!(ac >= 5 && ac <= 6))
		return (write(2, "nombre d'arguments\n", 19), 2);
	philo = init_philo(&info, philo, av, ac);
	start_philo(&info, philo);
	check_death(&info, philo);
	while (i < info.nb_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free(philo);
	
}
