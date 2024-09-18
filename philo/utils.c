/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:02:42 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/17 19:17:39 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	tmp;

	i = 0;
	sign = 1;
	tmp = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if ((str[i] == 43 || str[i] == 45))
		if (str[i++] == 45)
			sign *= -1;
	while (str[i] >= 48 && str[i] <= 57)
		tmp = (tmp * 10) + (str[i++] - 48);
	if ((tmp * sign < -2147483648 || tmp * sign > 2147483647))
		return (0);
	return (tmp * sign);
}

void	destroy_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philo[i].last_eat_m);
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_m);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((time.tv_sec * 1000) + (time.tv_usec / 1000)));
}

void	ft_usleep(long long time)
{
	long	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
}

int	is_valid_args(t_data *data)
{
	if (data->nbr_philo < 1)
		return (1);
	if (data->life_time < 60)
		return (1);
	if (data->eat_time < 60)
		return (1);
	if (data->sleep_time < 60)
		return (1);
	if (data->must_eat == 0)
		return (1);
	return (0);
}
