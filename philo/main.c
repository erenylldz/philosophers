/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:11:13 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/19 11:08:46 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 5 && ac != 6)
		return (1);
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->eat_count = 0;
	data->must_eat = -1;
	data->start_time = 0;
	if (av[1][0] == '0' || num_control(av, data) == 1)
	{
		free(data);
		return (1);
	}
	if (data_creat(av, data) == 1)
	{
		free(data);
		return (1);
	}
	main_thread(data);
	free(data->forks);
	free(data->philos);
	free(data);
	return (0);
}

void	main_thread(t_data *data)
{
	int	i;

	i = 1;
	while (1)
	{
		pthread_mutex_lock(&data->is_eat);
		if (data->must_eat != -1
			&& data->eat_count >= data->must_eat * data->number_philo)
		{
			pthread_mutex_lock(&data->write_mutex);
			return ;
		}
		data->cont = (i % data->number_philo);
		if (get_mss(data) - data->philos[data->cont].last_eat
			>= data->time_to_die)
		{
			pthread_mutex_lock(&data->write_mutex);
			printf("%lu %d died\n", get_mss(data), data->cont + 1);
			return ;
		}
		pthread_mutex_unlock(&data->is_eat);
		i++;
		usleep(100);
	}
}
