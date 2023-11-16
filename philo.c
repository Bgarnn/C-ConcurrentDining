#include "philo.h"

/* ************************************************************************** */
/* ******************************** utils.c ********************************* */

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

void	destroy_fork(t_data *data, pthread_mutex_t *lk_fork_arr, size_t fork_max)
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

/* ************************************************************************** */
/* ******************************** libft.c ********************************* */

#include "philo.h"

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
void	ft_putendl_fd(char *str, int fd)
{
	if (str == NULL || fd < 0)
		return ;
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
}

size_t	ft_atost(const char *str)
{
	size_t	num;

	num = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + *str - '0';
		str++;
	}
	return (num);
}

int	ft_isnum(int argc, char **argv)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (i < (size_t)argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (FALSE);
			else
				j++;
		}
		i++;
	}
	return (TRUE);
}

/* ************************************************************************** */
/* ***************************** thd_routine.c ****************************** */

#include "philo.h"

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

int	eat_max(t_ph_data *ph_arr)//can put in thd_monitor
{
	pthread_mutex_lock(ph_arr->lk_eat);
	if (ph_arr->eat_count == ph_arr->sj_eat_max)
		return (pthread_mutex_unlock(ph_arr->lk_eat), TRUE);
	return (pthread_mutex_unlock(ph_arr->lk_eat), FALSE);
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

/* ************************************************************************** */
/* ***************************** thd_monitor.c ****************************** */

#include "philo.h"


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
	if (time_now - ph_arr->time_eat_latest >= ph_arr->sj_die)
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

/* ************************************************************************** */
/* ******************************** philo.c ********************************* */

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
	if (data->sj_num == 1)
	{
		data->ph_arr[i].lk_fork_r = 0;
		data->ph_arr[i].lk_fork_l = 0;
		return(TRUE);
	}
	while (i < data->sj_num)
	{
		if (pthread_mutex_init(&lk_fk_array[i], NULL) != 0)
			return (destroy_fork(data, lk_fk_array, i),FALSE);
		i++;
	}
	i = 0;
	while (i < data->sj_num)
	{
		data->ph_arr[i].lk_fork_r = &lk_fk_array[i];
		data->ph_arr[i].lk_fork_l = &lk_fk_array[(i+1) % data->sj_num];
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

/* ************************************************************************** */