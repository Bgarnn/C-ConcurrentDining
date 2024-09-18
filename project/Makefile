# Program Name
NAME	= dining

# Compiler Properties 
CC		= cc
CFLAG	= -Wall -Werror -Wextra

RM		= rm -f

# File
DINING_DIR		= ./
DINING_HEADER	= ./dining.h
DINING_FILE	= dining.c lib.c thd_monitor.c thd_routine.c utils.c 
DINING_SRCS	= $(addprefix $(DINING_DIR), $(DINING_FILE))
DINING_OBJS	= $(DINING_SRCS:.c=.o)

# Compiler Process
.c.o:
	$(CC) $(CFLAG) -c $< -o $(<:.c=.o)

# Rules
.PHONY:	all clean fclean re

all:	$(NAME)

$(NAME):	$(DINING_OBJS) $(DINING_HEADER)
	$(CC) $(CFLAG) -lpthread $(DINING_OBJS) -o $(NAME)

clean:
	$(RM) $(DINING_OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean clean all
