/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:08:08 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/19 10:24:48 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_creat(t_data *data, bool ok)
{
	int	i;

	if (ok == true)
	{
		i = -1;
		while (++i < data->number_philo)
		{
			data->philos[i].id = i;
			data->philos[i].data = data;
			data->philos[i].is_dead = false;
			data->philos[i].last_eat = get_mss(data);
			if (pthread_create(&data->philos[i].thread, NULL,
					life, &data->philos[i]) != 0)
				return (-1);
			usleep(100);
		}
		return (-2);
	}
	return (0);
}

int	data_creat(char **str, t_data *data)
{
	int	i;

	data->time_to_eat = fatoi(str[3]);
	data->time_to_sleep = fatoi(str[4]);
	data->start_time = get_mss(data);
	if (fatoi(str[1]) == -2 || fatoi(str[2]) == -2 || fatoi(str[3]) == -2
		|| fatoi(str[4]) == -2 || (str[5] && fatoi(str[5]) == -2)
		|| (str[5] && fatoi(str[5]) == 0))
		return (1);
	if (str[5])
		data->must_eat = fatoi(str[5]);
	i = -1;
	if (pthread_mutex_init(&data->is_eat, NULL) != 0
		|| pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->number_philo);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_philo);
	if (!(data->philos) || !(data->forks))
		return (1);
	while (++i < data->number_philo)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
	if (philo_creat(data, true) == -1)
		return (1);
	return (0);
}

void	*life(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&p->data->forks[p->id]);
		print(p, 0);
		pthread_mutex_lock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		print(p, 0);
		print(p, 3);
		pthread_mutex_lock(&p->data->is_eat);
		p->data->eat_count++;
		p->last_eat = get_mss(p->data);
		pthread_mutex_unlock(&p->data->is_eat);
		ms_sleep(p, p->data->time_to_eat);
		print(p, 1);
		pthread_mutex_unlock(&p->data->forks[p->id]);
		pthread_mutex_unlock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		ms_sleep(p, p->data->time_to_sleep);
		print(p, 2);
	}
	return (NULL);
}

void	print(t_philo *p, int num)
{
	if (num == 0)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d has taken a fork\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 1)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is sleeping\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 2)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is thinking\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 3)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is eating\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
}
