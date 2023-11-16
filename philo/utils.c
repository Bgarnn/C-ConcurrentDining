/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:15:40 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 11:15:42 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	time_millisec(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	die_update(t_ph_data *ph_arr)
{
	pthread_mutex_lock(ph_arr->lk_die);
	if (*ph_arr->die_status == DIE)
		return (pthread_mutex_unlock(ph_arr->lk_die), DIE);
	return (pthread_mutex_unlock(ph_arr->lk_die), ALIVE);
}

void	ft_print(t_ph_data *ph_arr, char *str, char *color)
{
	pthread_mutex_lock(ph_arr->lk_print);
	if (die_update(ph_arr) == ALIVE)
	{
		printf("%s%zu  ", color, time_millisec() - ph_arr->time_ref);
		printf("%d %s\n", ph_arr->ph_ranging, str);
	}
	pthread_mutex_unlock(ph_arr->lk_print);
}

void	destroy_fork(t_data *data, pthread_mutex_t *lk_fork_arr, \
	size_t fork_max)
{
	size_t	i;

	i = 0;
	if (data->sj_num > 1)
	{
		while (i < fork_max)
			pthread_mutex_destroy(&lk_fork_arr[i++]);
	}
}

void	destroy_all(t_data *data)
{
	pthread_mutex_destroy(&data->lk_eat);
	pthread_mutex_destroy(&data->lk_die);
	pthread_mutex_destroy(&data->lk_print);
}
