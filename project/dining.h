#ifndef DINING_H
# define DINING_H

# define TRUE	1
# define FALSE	0
# define ALIVE	1
# define DIE	0

# define PH_MAX	250
# define RED	"\x1b[31;1m"
# define GREEN	"\x1b[32;1m"
# define YELLOW	"\x1b[33;1m"
# define BLUE	"\x1b[34;1m"

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_ph_data
{
	pthread_t		thd_philo;
	int				ph_ranging;
	int				eat_count;
	int				*die_status;
	int				sj_eat_max;
	size_t			sj_num;
	size_t			sj_die;
	size_t			sj_eat;
	size_t			sj_sleep;
	size_t			time_ref;
	size_t			time_eat_latest;
	pthread_mutex_t	*lk_die;
	pthread_mutex_t	*lk_eat;
	pthread_mutex_t	*lk_print;
	pthread_mutex_t	*lk_fork_r;
	pthread_mutex_t	*lk_fork_l;
}					t_ph_data;

typedef struct s_data
{
	pthread_t		thd_monitor;
	int				die_status;
	int				sj_eat_max;
	size_t			sj_num;
	size_t			time_ref;
	t_ph_data		*ph_arr;
	pthread_mutex_t	lk_eat;
	pthread_mutex_t	lk_die;
	pthread_mutex_t	lk_print;
}					t_data;

void	ft_putendl_fd(char *str, int fd);
size_t	ft_atost(const char *str);
int		ft_isnum(int argc, char **argv);
int		eat_max(t_ph_data *ph_arr);
void	*monitor(void *ptr);
void	*routine(void *var);
size_t	time_millisec(void);
int		die_update(t_ph_data *ph_arr);
void	ft_print(t_ph_data *ph_arr, char *str, char *color);
void	destroy_fork(t_data *data, pthread_mutex_t *lk_fork_arr, \
	size_t fork_max);
void	destroy_all(t_data *data);

#endif

/*
	pthread_mutex_t	*lk_die;
		(thd_monitor)
			ph_arr[i]->time_eat_latest :access
			ph_arr->die_status :access
			ph_arr->die_status :modify
		(thd_philo[i])
			ph_arr->die_status :access
			ph_arr[i]->time_eat_latest :modify

	pthread_mutex_t	*lk_eat;
		(thd_monitor)
			ph_arr[i]->eat_count :access
		(thd_philo[i])
			ph_arr[i]->eat_count :access
			ph_arr[i]->eat_count :modify

	pthread_mutex_t	*lk_print;
		(all thread)
			time & text
*/
