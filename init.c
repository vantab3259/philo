/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 10:36:01 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/11 13:14:40 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_philo(t_info *info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, &routine, &philo[i]))
		{
			while (i > 0)
			{
				i--;
				pthread_join(philo[i].thread, NULL);
				return (1);
			}	
		}
		i++;
	}
	return (0);
}

static void	init_info_part2(t_info *info, char **av)
{
	if (!ft_atoi(&info->nb_philo, av[1]))
	{
		printf("Error arguments\n");
		exit (1);
	}
	if (!ft_atoi(&info->time_to_die, av[2]))
	{
		printf("Error arguments\n");
		exit (1);
	}
	if (!ft_atoi(&info->time_to_eat, av[3]))
	{
		printf("Error arguments\n");
		exit (1);
	}
	if (!ft_atoi(&info->time_to_sleep, av[4]))
	{
		printf("Error arguments\n");
		exit (1);
	}
	info->dead = 0;
	info->finished = 0;
}

static void	init_info(t_info *info, char **av, int ac)
{
	init_info_part2(info, av);
	if (info->nb_philo <= 0 || info->time_to_die <= 0
		|| info->time_to_eat <= 0 || info->time_to_sleep <= 0)
	{
		printf("Error arguments\n");
		exit (1);
	}
	if (ac == 6)
	{
		info->max_eat = atoi(av[5]);
		if (info->max_eat <= 0)
		{
			printf("Error arguments\n");
			exit(1);
		}
	}
	else
		info->max_eat = 0;
}

t_philo	*init_philo(t_info *info, t_philo *philo, char **av, int ac)
{
	int	i;

	i = 0;
	init_info(info, av, ac);
	philo = calloc(sizeof(t_philo), info->nb_philo);
	info->time_start = gettime();
	pthread_mutex_init(&info->print, NULL);
	pthread_mutex_init(&info->finish, NULL);
	pthread_mutex_init(&info->death, NULL);
	while (i < info->nb_philo)
	{
		philo[i].info = info;
		philo[i].id = i;
		philo[i].last_meal = info->time_start;
		philo[i].count_meal = 0;
		pthread_mutex_init(&philo[i].fork, NULL);
		pthread_mutex_init(&philo[i].eating, NULL);
		philo[i].fork_left = &philo[i].fork;
		if (i == info->nb_philo - 1)
			philo[i].fork_right = &philo[0].fork;
		else
			philo[i].fork_right = &philo[i + 1].fork;
		i++;
	}
	return (philo);
}
