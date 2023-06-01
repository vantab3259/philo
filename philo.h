/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 01:41:05 by mudoh             #+#    #+#             */
/*   Updated: 2023/05/16 19:02:26 by mudoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_info
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	tmax_to_eat;
	long	time_start;
	int		dead;
	int		finished;
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t	finish;
} t_info;

typedef struct s_philo
{
	t_info			*info;
	int				count_meal;
	long			last_meal;
	// int				finish;
	int				id;
	pthread_mutex_t	eating;
	pthread_mutex_t	fork;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_t		thread;
} 			t_philo;

long	gettime(void);
void	ft_usleep(long ms);
void	print(t_philo *philo, char *str);
int		is_dead(t_info	*info);
#endif