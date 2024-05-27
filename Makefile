# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:11:35 by ting              #+#    #+#              #
#    Updated: 2024/05/27 17:06:53 by ting             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

FLAGS = -Wall -Wextra -Werror -g

RM = rm -rf

LIBFT = libft/libft.a

SRCSPATH = srcs/

SRCS = minishell.c

OBJS = $(addprefix $(SRCSPATH), $(SRCS:.c=.o))

$(NAME): $(OBJS)
			@$(MAKE) -C libft
			@cc $(FLAGS) $(LIBFT) $(OBJS) -lreadline -o $(NAME)
all: $(NAME)

clean: 
			@$(MAKE) clean -C libft
			@$(RM) $(OBJS)	

fclean: clean
			@$(MAKE) fclean -C libft
			@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re
