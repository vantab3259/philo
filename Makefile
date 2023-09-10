# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mudoh <mudoh@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 05:24:25 by mudoh             #+#    #+#              #
#    Updated: 2023/09/08 19:18:02 by mudoh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = main.c\
	routine.c\
	utils.c\
	init.c

OBJS = ${SRCS:.c=.o}

CFLAGS = -g3 -Wall -Werror -Wextra

all: ${NAME}

${NAME}: ${OBJS}
	cc -g3 ${CFLAGS} ${OBJS} -o ${NAME}
	
clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${NAME}

re : fclean all

.PHONY: clean fclean all re	

