# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/08 10:29:07 by rapohlen          #+#    #+#              #
#    Updated: 2025/12/15 22:57:07 by rapohlen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES				= main.c \
					  pipex.c \
					  usage.c \
					  error.c \
					  open.c heredoc.c \
					  pipe.c argv.c path.c \
					  util.c lst_util.c \
					  init.c exit.c

SRCDIR				= src
BUILDDIR			= .build

SRC					= $(addprefix $(SRCDIR)/, $(CFILES))
OBJ					= $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEP					= $(OBJ:.o=.d)

NAME				= pipex

INC					= inc \
					  libft/inc
LIB					= libft/libft.a

CC					= cc
CFLAGS				= -Wall -Wextra -Werror
CPPFLAGS			= $(addprefix -I,$(INC)) -MMD -MP
MAKEFLAGS			+= --no-print-directory -j

all:				$(NAME)

bonus:				$(NAME)

$(NAME):			$(OBJ) $(LIB)
					$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

$(LIB):
					$(MAKE) -C $(@D)

$(BUILDDIR)/%.o:	$(SRCDIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
					@for f in $(dir $(LIB)); do $(MAKE) -C $$f clean; done
					rm -rf $(BUILDDIR)

fclean:
					@for f in $(dir $(LIB)); do $(MAKE) -C $$f fclean; done
					rm -rf $(NAME) $(BUILDDIR)

re:
					$(MAKE) fclean
					$(MAKE) all

-include $(DEP)

.PHONY: clean fclean re
