/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thd_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:15:06 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 11:15:08 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat_max(t_ph_data *ph_arr)
{
	pthread_mutex_lock(ph_arr->lk_eat);
	if (ph_arr->eat_count == ph_arr->sj_eat_max)
		return (pthread_mutex_unlock(ph_arr->lk_eat), TRUE);
	return (pthread_mutex_unlock(ph_arr->lk_eat), FALSE);
}

static size_t	eat_max_check(t_data *data)
{
	size_t	i;
	size_t	eat_max_count;

	i = 0;
	eat_max_count = 0;
	while (i < data->sj_num)
	{
		if (eat_max(&data->ph_arr[i]) == TRUE)
			eat_max_count++;
		i++;
	}
	if (eat_max_count == data->sj_num)
		return (TRUE);
	return (FALSE);
}

static int	die_already(t_ph_data *ph_arr)
{
	size_t	time_now;

	pthread_mutex_lock(ph_arr->lk_die);
	time_now = time_millisec();
	if (ph_arr->sj_die <= time_now - ph_arr->time_eat_latest)
		return (pthread_mutex_unlock(ph_arr->lk_die), DIE);
	return (pthread_mutex_unlock(ph_arr->lk_die), ALIVE);
}

static size_t	die_check(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->sj_num)
	{
		if (die_already(&data->ph_arr[i]) == DIE)
		{
			ft_print(&data->ph_arr[i], "died", RED);
			pthread_mutex_lock(&data->lk_die);
			data->die_status = DIE;
			return (pthread_mutex_unlock(&data->lk_die), DIE);
		}
		i++;
	}
	return (ALIVE);
}

void	*monitor(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	while (1)
	{
		usleep(1);
		if (die_check(data) == DIE)
			break ;
		if (eat_max_check(data) == TRUE)
			break ;
	}
	return (0);
}
