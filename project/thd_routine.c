#include "dining.h"

static void	delay_millisec(size_t millisec)
{
	size_t	start;

	start = time_millisec();
	while (time_millisec() - start < millisec)
		usleep(1);
}

static void	ph_eating(t_ph_data *ph_arr)
{
	pthread_mutex_lock(ph_arr->lk_fork_r);
	ft_print(ph_arr, "has taken a fork", GREEN);
	pthread_mutex_lock(ph_arr->lk_fork_l);
	ft_print(ph_arr, "has taken a fork", GREEN);
	pthread_mutex_lock(ph_arr->lk_eat);
	ph_arr->eat_count += 1;
	pthread_mutex_unlock(ph_arr->lk_eat);
	pthread_mutex_lock(ph_arr->lk_die);
	ph_arr->time_eat_latest = time_millisec();
	pthread_mutex_unlock(ph_arr->lk_die);
	ft_print(ph_arr, "is eating", GREEN);
	delay_millisec(ph_arr->sj_eat);
	pthread_mutex_unlock(ph_arr->lk_fork_r);
	pthread_mutex_unlock(ph_arr->lk_fork_l);
}

static void	ph_sleeping(t_ph_data *ph_arr)
{
	ft_print(ph_arr, "is sleeping", BLUE);
	delay_millisec(ph_arr->sj_sleep);
}

static void	ph_thinking(t_ph_data *ph_arr)
{
	ft_print(ph_arr, "is thinking", YELLOW);
}

void	*routine(void *var)
{
	t_ph_data	*ph_arr;

	ph_arr = var;
	if (ph_arr->ph_ranging % 2 == 0)
		delay_millisec(2);
	while (1)
	{
		if (die_update(ph_arr) == DIE || eat_max(ph_arr) == TRUE)
			break ;
		if (ph_arr->sj_num == 1)
		{
			delay_millisec(ph_arr->sj_die);
			break ;
		}
		ph_eating(ph_arr);
		ph_sleeping(ph_arr);
		ph_thinking(ph_arr);
	}
	return (0);
}
