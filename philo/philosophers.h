/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:08:04 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/19 10:18:22 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct philos
{
	int				id;
	long			last_eat;
	bool			is_dead;
	pthread_t		thread;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	int				number_philo;
	int				cont;
	int				must_eat;
	int				eat_count;
	t_philo			*philos;
	struct timeval	tv;
	pthread_mutex_t	*forks;
	pthread_mutex_t	is_eat;
	pthread_mutex_t	write_mutex;
}	t_data;

long	fatoi(char *str);
int		philo_creat(t_data *data, bool ok);
long	get_mss(t_data *data);
int		data_creat(char **str, t_data *data);
int		num_control(char **str, t_data *data);
void	*life(void *philo);
void	print(t_philo *p, int num);
void	main_thread(t_data *data);
void	ms_sleep(t_philo *p, long time);
#endif
