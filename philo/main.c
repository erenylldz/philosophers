/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:11:13 by eryildiz          #+#    #+#             */
/*   Updated: 2024/06/12 19:34:35 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	data_init(t_data *data, char **av)
{
	data->eat_count = 0;
	data->nbr_philo = ft_atoi(av[1]);
	data->timer.t_die = ft_atoi(av[2]);
	data->timer.eating = ft_atoi(av[3]);
	data->timer.sleeping = ft_atoi(av[4]);
	data->timer.max_eat = -1;
	if (av[5] != NULL)
		data->timer.max_eat = ft_atoi(av[5]);
	if (data->timer.max_eat == 0)
		return (1);
	if (pthread_mutex_init(&data->is_eat, NULL) != 0)
		return (1);
	data->eat_bool = true;
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	data->write_bool = true;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks)
		return (1);
	data->philo = malloc(sizeof(t_philo) * data->nbr_philo);
	if (!data->philo)
		return (free_mutex_philos(data), 1);
	if (start_forks_philos(data) == 1)
		return (free_mutex_philos(data), 1);
}

void	free_mutex_philos(t_data *data)
{
	int	i;

	i = -1;
	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
	if (data->eat_bool == true)
		pthread_mutex_destroy(&data->is_eat);
	if (data->write_bool == true)
		pthread_mutex_destroy(&data->write_mutex);
	while (++i <= data->nbr_philo)
		if (data->forks_bool[i] == true)
			pthread_mutex_destroy(&data->forks[i]);
}

int	start_forks_philos(t_data *data)
{
	int	i;

	i = -1;
	data->forks_bool = malloc(sizeof(bool) * data->nbr_philo);
	if (!data->forks_bool)
		return (1);
	while (++i <= data->nbr_philo)
		data->forks_bool[i] = false;
	i = -1;
	while (++i <= data->nbr_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		data->forks_bool[i] = true;
	}
	i = -1;
	while (++i < data->nbr_philo)
	{
		data->philo[i].philo_inx = i;
		data->philo[i].last_meat = updater(data);
		data->philo[i].philo_data = data;
		if (pthread_create(&data->philo->thread, NULL,
				philo_life, data->philo) != 0)
			return (free_mutex_philos(data), 1);
	}
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 5 || ac != 6)
		return (0);
	data = malloc(sizeof(t_data));
	if (!data)
		return ;
	data->write_bool = false;
	data->eat_bool = false;
	if (arg_check(av) == 1)
		return (free_mutex_philos(data), 1);
	if (data_init(data, av) == 1)
		return (free_mutex_philos(data), 1);
}

int	arg_check(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		if (ft_atoi(av[i]) < 0)
			return (1);
		if (ft_atoi(av[i]) > 2147483647)
			return (1);
		i++;
	}
	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (1);
			j++;
		}
		i++;
	}
}

void	main_loop(t_data *data)
{
	int	i;

	i = 1;
	(void)data;
	while (1)
	{
		pthread_mutex_lock(&data->is_eat);
		if (data->timer.max_eat != 1 && data->eat_count
			>= data->timer.max_eat * data->nbr_philo)
		{
			pthread_mutex_lock(&data->write_mutex);
			return ;
		}
		if (updater(data) - data->philo[i % data->nbr_philo].last_meat
			>= data->timer.t_die)
		{
			pthread_mutex_lock(&data->write_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->is_eat);
		i++;
		usleep(100);
	}
}
