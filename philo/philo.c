/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kburalek <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:14:52 by kburalek          #+#    #+#             */
/*   Updated: 2023/11/16 11:14:54 by kburalek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_init(t_data *data, t_ph_data *ph_arr, char **argv)
{
	size_t	i;

	i = 0;
	while (i < data->sj_num)
	{
		ph_arr[i].thd_philo = 0;
		ph_arr[i].ph_ranging = i + 1;
		ph_arr[i].eat_count = 0;
		ph_arr[i].die_status = &data->die_status;
		ph_arr[i].sj_eat_max = data->sj_eat_max;
		ph_arr[i].sj_num = data->sj_num;
		ph_arr[i].sj_die = ft_atost(argv[2]);
		ph_arr[i].sj_eat = ft_atost(argv[3]);
		ph_arr[i].sj_sleep = ft_atost(argv[4]);
		ph_arr[i].time_ref = data->time_ref;
		ph_arr[i].time_eat_latest = data->time_ref;
		ph_arr[i].lk_die = &data->lk_die;
		ph_arr[i].lk_eat = &data->lk_eat;
		ph_arr[i].lk_print = &data->lk_print;
		i++;
	}
	return (TRUE);
}

static int	data_init(t_data *data, t_ph_data *ph_arr, int argc, char **argv)
{
	data->thd_monitor = 0;
	data->die_status = ALIVE;
	data->sj_eat_max = -1;
	data->sj_num = ft_atost(argv[1]);
	data->time_ref = time_millisec();
	data->ph_arr = ph_arr;
	if (argc == 6)
		data->sj_eat_max = (int)ft_atost(argv[5]);
	if (pthread_mutex_init(&data->lk_eat, NULL) != 0)
		return (FALSE);
	if (pthread_mutex_init(&data->lk_die, NULL) != 0)
		return (pthread_mutex_destroy(&data->lk_eat), FALSE);
	if (pthread_mutex_init(&data->lk_print, NULL) != 0)
		return (pthread_mutex_destroy(&data->lk_eat), \
			pthread_mutex_destroy(&data->lk_die), FALSE);
	if (philo_init(data, ph_arr, argv) == FALSE)
		return (destroy_all(data), ft_putendl_fd("Philo: Philo incorrect", 2), \
			FALSE);
	return (TRUE);
}

static int	fork_init(t_data *data, pthread_mutex_t *lk_fk_array)
{
	size_t	i;

	i = 0;
	while (i < data->sj_num)
	{
		if (pthread_mutex_init(&lk_fk_array[i], NULL) != 0)
			return (destroy_fork(data, lk_fk_array, i), FALSE);
		i++;
	}
	if (data->sj_num == 1)
	{
		data->ph_arr[i].lk_fork_r = 0;
		data->ph_arr[i].lk_fork_l = 0;
		pthread_mutex_destroy(&lk_fk_array[0]);
		return (TRUE);
	}
	i = 0;
	while (i < data->sj_num)
	{
		data->ph_arr[i].lk_fork_r = &lk_fk_array[i];
		data->ph_arr[i].lk_fork_l = &lk_fk_array[(i + 1) % data->sj_num];
		i++;
	}
	return (TRUE);
}

static int	thread_init(t_data *data)
{
	size_t	i;

	if (pthread_create(&data->thd_monitor, NULL, \
		&monitor, data) != 0)
		return (FALSE);
	i = 0;
	while (i < data->sj_num)
	{
		if (pthread_create(&data->ph_arr[i].thd_philo, NULL, \
			&routine, &data->ph_arr[i]) != 0)
			return (FALSE);
		i++;
	}
	if (pthread_join(data->thd_monitor, NULL) != 0)
		return (FALSE);
	i = 0;
	while (i < data->sj_num)
	{
		if (pthread_join(data->ph_arr[i++].thd_philo, NULL) != 0)
			return (FALSE);
	}
	return (TRUE);
}

int	main(int argc, char **argv)
{
	t_data			data;
	t_ph_data		ph_arr[PH_MAX];
	pthread_mutex_t	lk_fork_arr[PH_MAX];

	if ((argc != 5 && argc != 6) || ft_isnum(argc, argv) == FALSE)
		return (ft_putendl_fd("Philo: Input incorrect", 2), EXIT_FAILURE);
	if (data_init(&data, ph_arr, argc, argv) == FALSE)
		return (ft_putendl_fd("Philo: Data incorrect", 2), EXIT_FAILURE);
	if (fork_init(&data, lk_fork_arr) == FALSE)
		return (destroy_all(&data), \
			ft_putendl_fd("Philo: Fork incorrect", 2), EXIT_FAILURE);
	if (thread_init(&data) == FALSE)
	{
		destroy_fork(&data, lk_fork_arr, data.sj_num);
		destroy_all(&data);
		return (ft_putendl_fd("Philo: Thread incorrect", 2), EXIT_FAILURE);
	}
	destroy_fork(&data, lk_fork_arr, data.sj_num);
	destroy_all(&data);
	return (EXIT_SUCCESS);
}
