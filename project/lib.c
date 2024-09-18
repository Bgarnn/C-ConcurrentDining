#include "dining.h"

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
