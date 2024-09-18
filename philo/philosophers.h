/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eryildiz <eryildiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:08:04 by eryildiz          #+#    #+#             */
/*   Updated: 2024/09/17 18:21:38 by eryildiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DEAD "died"

typedef struct s_philo
{
	char			*step;
	int				idx;
	int				fork1;
	int				fork2;
	int				eat_count;
	long			last_eat;
	long			start;
	pthread_mutex_t	last_eat_m;
	pthread_t		thread;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				life_time;
	int				nbr_philo;
	int				sleep_time;
	int				eat_time;
	int				must_eat;
	int				dead;
	int				is_dead;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_m;
	pthread_mutex_t	philo_count;
	t_philo			*philo;
}					t_data;

int		check_arg(char	**av);
void	data_init(t_data *data, char **av);
void	init_philo(t_data *data);
int		start_thread(t_data *data);
void	tracking_thread(t_data *data);
void	print_step(t_philo *philo, char *step);
void	*routine(void	*ptr);
void	philo_loop(t_data *data, t_philo *philo);
int		check_eat(t_philo *philo, t_data *data);
long	ft_atol(const char *str);
void	destroy_data(t_data *data);
long	get_time(void);
void	ft_usleep(long long time);
int		is_valid_args(t_data *data);
#endif
