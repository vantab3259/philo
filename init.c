/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 10:36:01 by mudoh             #+#    #+#             */
/*   Updated: 2023/09/08 11:12:30 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
static void init_info(t_info *info, char **av, int ac)
{
	info->nb_philo = atoi(av[1]);
	info->time_to_die = atoi(av[2]);
	info->time_to_eat = atoi(av[3]);
	info->time_to_sleep  = atoi(av[4]);
	if (info->nb_philo <= 0 || info->time_to_die <= 0
		|| info->time_to_eat <= 0 || info->time_to_sleep <= 0)
	{
		printf("Error arguments\n");
		exit(1);
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
	if (ac == 6)
		info->max_eat = atoi(av[5]);
	else 
		info->max_eat = 0; // 0?
}

t_philo *init_philo(t_info *info, t_philo *philo, char **av, int ac)
{
	init_info(info, av, ac);
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