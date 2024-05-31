# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:11:35 by ting              #+#    #+#              #
#    Updated: 2024/05/31 16:57:46 by ting             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

FLAGS = -Wall -Wextra -Werror #-g

RM = rm -rf

LIBFT = libft/libft.a

SRCSPATH = srcs/

SRCS = minishell.c parsing.c \
		lexer.c expand_env_var.c \
		free_for_lexer.c

OBJS = $(addprefix $(SRCSPATH), $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
		@$(MAKE) -C libft
		@cc $(FLAGS) -g $(OBJS) $(LIBFT) -lreadline -o $(NAME)


clean:
		@$(MAKE) clean -C libft
		@$(RM) $(OBJS)

fclean: clean
		@$(MAKE) fclean -C libft
		@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re
