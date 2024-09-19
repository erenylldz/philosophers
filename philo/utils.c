/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:02:42 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/19 10:23:20 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	fatoi(char *str)
{
	long	num;
	long	sign;
	int		i;

	i = 0;
	num = 0;
	sign = 1;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num += str[i] - 48;
		i++;
	}
	num *= sign;
	if (num > 2147483647 || num < -2147483648)
		return (-2);
	return (sign * num);
}

int	num_control(char **str, t_data *data)
{
	int	i;
	int	j;

	j = 1;
	while (str[j])
	{
		i = 0;
		while (str[j][i] && (str[j][i] == 32
			|| (str[j][i] >= 9 && str[j][i] <= 13)))
			i++;
		while (str[j][i])
		{
			if (str[j][i] >= '0' && str[j][i] <= '9')
				i++;
			else
				return (1);
		}
		j++;
	}
	data->eat_count = 0;
	data->must_eat = -1;
	data->start_time = 0;
	data->number_philo = (int)fatoi(str[1]);
	data->time_to_die = fatoi(str[2]);
	return (0);
}

long	get_mss(t_data *data)
{
	long	t;

	t = data->start_time;
	gettimeofday(&data->tv, NULL);
	return ((data->tv.tv_usec / 1000 + data->tv.tv_sec * 1000) - t);
}

void	ms_sleep(t_philo *p, long time)
{
	long	dest_time;

	dest_time = get_mss(p->data) + time;
	while (get_mss(p->data) < dest_time)
		usleep(100);
}
