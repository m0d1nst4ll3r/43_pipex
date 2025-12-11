# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/08 10:29:07 by rapohlen          #+#    #+#              #
#    Updated: 2025/12/11 20:41:55 by rapohlen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Ilibft/inc -g

SRC			= main.c \
			  pipex.c \
			  usage.c \
			  error.c \
			  open.c heredoc.c \
			  pipe.c argv.c path.c \
			  util.c lst_util.c \
			  init.c exit.c

OBJ			= $(SRC:.c=.o)

NAME		= pipex

LIB			= libft/libft.a

all:		$(NAME)

bonus:		all

$(NAME):	$(OBJ) $(LIB) 
			$(CC) $(CFLAGS) -o $@ $^

$(LIB):
			$(MAKE) -C libft

clean:
			$(MAKE) clean -C libft
			rm -f $(OBJ)

fclean:		clean
			$(MAKE) fclean -C libft
			rm -f $(NAME)

re:			fclean all

.PHONY: clean fclean all
