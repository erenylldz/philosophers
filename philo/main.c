/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:11:13 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/17 18:26:55 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (1);
	if (check_arg(av))
		return (1);
	data_init(&data, av);
	if (is_valid_args(&data))
	{
		destroy_data(&data);
		free(data.forks);
		free(data.philo);
		return (1);
	}
	if (start_thread(&data))
	{
		destroy_data(&data);
		free(data.forks);
		free(data.philo);
		return (1);
	}
	destroy_data(&data);
	free(data.forks);
	free(data.philo);
	return (0);
}

int	check_arg(char	**av)
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] < 48 || av[i][j] > 57)
				return (printf("Error: Invalid argument\n"), 1);
		}
	}
	return (0);
}

void	data_init(t_data *data, char **av)
{
	data->nbr_philo = ft_atol(av[1]);
	data->life_time = ft_atol(av[2]);
	data->eat_time = ft_atol(av[3]);
	data->sleep_time = ft_atol(av[4]);
	if (av[5] != NULL)
		data->must_eat = ft_atol(av[5]);
	else
		data->must_eat = -1;
	data->is_dead = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	data->philo = malloc(sizeof(t_philo) * data->nbr_philo);
	pthread_mutex_init(&data->dead_m, NULL);
	pthread_mutex_init(&data->philo_count, NULL);
	init_philo(data);
}

void	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_philo)
	{
		data->philo[i].idx = i;
		data->philo[i].eat_count = 0;
		data->philo[i].step = NULL;
		data->philo[i].data = data;
		data->philo[i].last_eat = get_time();
		data->philo[i].start = get_time();
		data->philo[i].fork1 = i;
		data->philo[i].fork2 = (i + 1) % data->nbr_philo;
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philo[i].last_eat_m, NULL);
	}
	pthread_mutex_init(&data->print, NULL);
}

void	print_step(t_philo *philo, char *step)
{
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->dead_m);
	if (philo->data->is_dead)
	{
		pthread_mutex_lock(&philo->data->dead_m);
		pthread_mutex_lock(&philo->data->print);
		return ;
	}
	pthread_mutex_unlock(&philo->data->dead_m);
	printf("%ld %d %s\n", get_time() - philo->start, philo->idx + 1, step);
	pthread_mutex_unlock(&philo->data->print);
}
