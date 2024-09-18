/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:08:08 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/17 19:30:25 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_thread(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_philo)
	{
		if (pthread_create(&data->philo[i].thread,
				NULL, routine, &data->philo[i]))
			return (1);
	}
	tracking_thread(data);
	i = -1;
	while (++i < data->nbr_philo)
	{
		if (pthread_join(data->philo[i].thread, NULL))
			return (1);
	}
	return (0);
}

void	tracking_thread(t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		if (i >= data->nbr_philo)
			i = 0;
		pthread_mutex_lock(&data->philo[i].last_eat_m);
		if (data->must_eat != -1 && data->philo[i].eat_count == data->must_eat)
		{
			pthread_mutex_unlock(&data->philo[i].last_eat_m);
			break ;
		}
		if ((get_time() - data->philo[i].last_eat >= data->life_time))
		{
			print_step(&data->philo[i], DEAD);
			pthread_mutex_lock(&data->dead_m);
			data->is_dead = 1;
			pthread_mutex_unlock(&data->dead_m);
			pthread_mutex_unlock(&data->philo[i].last_eat_m);
			break ;
		}
		pthread_mutex_unlock(&data->philo[i].last_eat_m);
		i++;
	}
}

void	*routine(void	*ptr)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)ptr;
	data = philo->data;
	pthread_mutex_lock(&philo->last_eat_m);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->last_eat_m);
	if (philo->idx % 2)
	{
		ft_usleep(data->eat_time);
		pthread_mutex_lock(&philo->last_eat_m);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->last_eat_m);
	}
	philo_loop(data, philo);
	return (NULL);
}

int	check_eat(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->forks[philo->fork1]);
	print_step(philo, FORK);
	if (philo->fork1 == philo->fork2)
	{
		ft_usleep(data->life_time);
		pthread_mutex_unlock(&data->forks[philo->fork1]);
		return (1);
	}
	pthread_mutex_lock(&data->forks[philo->fork2]);
	print_step(philo, FORK);
	print_step(philo, EATING);
	pthread_mutex_lock(&philo->last_eat_m);
	philo->last_eat = get_time();
	if (data->must_eat != -1)
		philo->eat_count++;
	pthread_mutex_unlock(&philo->last_eat_m);
	ft_usleep(data->eat_time);
	pthread_mutex_unlock(&data->forks[philo->fork2]);
	pthread_mutex_unlock(&data->forks[philo->fork1]);
	return (0);
}

void	philo_loop(t_data *data, t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&data->dead_m);
		if (data->is_dead)
		{
			pthread_mutex_unlock(&data->dead_m);
			break ;
		}
		if (data->must_eat != -1 && philo->eat_count == data->must_eat)
		{
			pthread_mutex_unlock(&data->dead_m);
			break ;
		}
		pthread_mutex_unlock(&data->dead_m);
		if (!check_eat(philo, data))
		{
			printf("1234567544564575443454676\n");
			if (data->must_eat && data->must_eat == philo->eat_count)
				break ;
			print_step(philo, SLEEPING);
			ft_usleep(data->sleep_time);
			print_step(philo, THINKING);
		}
	}
}
