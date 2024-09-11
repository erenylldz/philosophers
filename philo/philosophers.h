/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:08:04 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/11 18:01:31 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct s_time
{
	long	eating;
	long	sleeping;
	long	t_die;
	long	s_time;
	long	max_eat;
}	t_time;

typedef struct s_data
{
	struct s_philosophers	*philo;
	t_time					timer;
	long					eat_count;
	int						nbr_philo;
	struct timeval			tv;
	pthread_mutex_t			*forks;
	pthread_mutex_t			is_eat;
	pthread_mutex_t			write_mutex;
	bool					write_bool;
	bool					eat_bool;
	bool					*forks_bool;
}	t_data;

typedef struct s_philosophers
{
	int			philo_inx;
	long		last_meat;
	int			is_dead;
	pthread_t	thread;
	t_data		*philo_data;
}	t_philo;

void			free_mutex_philos(t_data *data);
long			updater(t_data *data);
long			ft_atoi(const char *str);
int				data_init(t_data *data, char **av);
void			ms_slepp(t_data *data, unsigned long time);
void			*philo_life(void *philo);
int				start_forks_philos(t_data *data);
int				arg_check(char **av);
#endif
