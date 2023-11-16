# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kburalek <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/16 11:18:24 by kburalek          #+#    #+#              #
#    Updated: 2023/11/16 11:18:25 by kburalek         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME	= philo

# Compiler Properties 
CC		= cc
CFLAG	= -Wall -Werror -Wextra

RM		= rm -f

# File
PHILO_DIR		= ./
PHILO_HEADER	= ./philo.h
PHILO_FILE	= philo.c libft.c thd_monitor.c thd_routine.c utils.c 
PHILO_SRCS	= $(addprefix $(PHILO_DIR), $(PHILO_FILE))
PHILO_OBJS	= $(PHILO_SRCS:.c=.o)

# Compiler Process
.c.o:
	$(CC) $(CFLAG) -c $< -o $(<:.c=.o)

# Rules
.PHONY:	all clean fclean re

all:	$(NAME)

$(NAME):	$(PHILO_OBJS) $(PHILO_HEADER)
	$(CC) $(CFLAG) -lpthread $(PHILO_OBJS) -o $(NAME)

clean:
	$(RM) $(PHILO_OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean clean all
