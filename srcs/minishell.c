/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:58:54 by ting              #+#    #+#             */
/*   Updated: 2024/06/11 18:21:56 by ting             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_cmd	**cmds;
	t_env	**env_dup;
	t_cmd	*current;

	(void)ac;
	(void)av;

	env_dup = init_env_copy(env);
	if (env_dup == NULL)
	{
		perror ("init_env_copy");
		return (1);
	}
	cmds = (t_cmd **)malloc(sizeof(t_cmd *));
	*cmds = NULL;
	while (1)
	{
		line = readline(C "shell@st42:$ " RST); 
		if (line == NULL)
			break ; // exit if EOF or error, can be Ctrl + D
		if (line)
		{
			if (lexer_and_parse(cmds, line))
			{
				free_cmds(cmds);
				free(line);
				continue;
			}
			free(line);
			current = *cmds;
			while(current)
			{
				if (current->builtin)
					execute_builtins(cmds, current->cmd_arr, env_dup);
				current = current->next;
			}
			free_cmds(cmds);
		}
	}
	return (0);
}
