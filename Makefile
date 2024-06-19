# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:11:35 by ting              #+#    #+#              #
#    Updated: 2024/06/11 19:13:13 by ting             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

FLAGS = -Wall -Wextra -Werror -g

RM = rm -rf

LIBFT = libft/libft.a

SRCSPATH = srcs/

SRCS = minishell.c builtins_env_node_utils.c builtin_env.c\
		builtins_utils.c builtins.c builtin_exit.c\
		builtin_export.c builtin_unset.c\
		parsing.c lexer.c lexer_utils.c remove_quotes.c\
		expand_env_var.c parsing_utils.c \
		free.c print_error_msg.c

OBJS = $(addprefix $(SRCSPATH), $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
		@$(MAKE) -C libft
		@cc $(FLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(SRCSPATH)%.o: $(SRCSPATH)%.c
	cc $(FLAGS) -Iincludes -c $< -o $@

clean:
		@$(MAKE) clean -C libft
		@$(RM) $(OBJS)

fclean: clean
		@$(MAKE) fclean -C libft
		@$(RM) $(NAME)

re: fclean all

.phony: all clean fclean re
