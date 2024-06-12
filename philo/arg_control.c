/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:08:08 by eryildiz          #+#    #+#             */
/*   Updated: 2024/06/12 17:46:35 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(const char *str)
{
	long	i;
	long	negative;

	negative = 1;
	i = 0;
	while (*str == ' ' || *str == '\n' || *str == '\t'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '+' || *str == '-')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		i = i * 10 + (*str - 48);
		++str;
	}
	return (i * negative);
}

unsigned long	updater(t_data *data)
{
	unsigned long	time;

	time = data->timer.s_time;
	gettimeofday(&data->tv, NULL);
	time = (data->tv.tv_usec / 1000 + data->tv.tv_sec * 1000) - time;
	return (time);
}

void	ms_slepp(t_data *data, unsigned long time)
{
	unsigned long	life;

	life = updater(data) + time;
	while (life > updater(data))
		usleep(31);
}

void	*philo_life(void *ph)
{
	t_philo	*philo;

	philo = (t_philo *)ph;
	while (1)
	{
		pthread_mutex_lock(&philo->philo_data->forks[philo->philo_inx]);
		printf("%d has taken a fork\n", philo->philo_inx + 1);
		pthread_mutex_lock(&philo->philo_data->forks[philo->philo_inx + 1]);
		printf("%d has taken a fork\n", philo->philo_inx + 1);
		printf("%d has eating\n", philo->philo_inx + 1);
		pthread_mutex_lock(&philo->philo_data->is_eat);
		philo->philo_data->eat_count++;
		philo->last_meat = updater(philo->philo_data);
		pthread_mutex_unlock(&philo->philo_data->is_eat);
		ms_slepp(philo->philo_data, philo->philo_data->timer.eating);
		printf("%d has sleeping\n", philo->philo_inx + 1);
		pthread_mutex_unlock(&philo->philo_data->forks[philo->philo_inx]);
		pthread_mutex_unlock(&philo->philo_data->forks[philo->philo_inx + 1]);
		ms_slepp(philo->philo_data, philo->philo_data->timer.sleeping);
		printf("%d is thinking \n", philo->philo_inx);
	}
}
