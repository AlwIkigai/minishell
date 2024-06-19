/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 13:58:54 by ting              #+#    #+#             */
/*   Updated: 2024/06/19 19:07:07 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
void	signal_handlers_setup()

{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask)
	
	sa_int.sa_handler = sigint_handler;
}
*/

/*
int	main(int ac, char **av, char **env)
{
	char		*line;
	t_cmd		**cmds;
	t_env		**env_dup;
	t_cmd		*current;
	t_ms_state	*status;

	(void)ac;
	(void)av;
	//if (signal_handlers_setup() != 0)
	//	return (1);
	status = init_status();
	env_dup = init_envdup(status, env);
	cmds = (t_cmd **)malloc(sizeof(t_cmd *));
	*cmds = NULL;
	while (1)
	{
		line = readline(C "shell@st42:$ " RST);
		if (line == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO); // Handle Ctrl + D
			rl_clear_history();
			break ; // exit if EOF or error, can be Ctrl + D
		}
		if (line && *line)
		{
			add_history(line);
			if (lexer_and_parse(cmds, line, env_dup))
			{
				free_cmds(cmds);
				free(line); //prob dont need to free, only in exit
				continue;
			}
			free(line);
			current = *cmds;
			while(current)
			{
				if (current->builtin)
					execute_builtins(cmds, current->cmd_arr, env_dup, status);
				current = current->next;
			}
			free_cmds(cmds);
		}
	}
	return (status->exit_status);
}*/
int	main(int ac, char **av, char **env)
{
	char		*line;
	t_cmd		**cmds;
	t_env		**env_dup;
	t_cmd		*current;
	t_ms_state	*update;

	(void)ac;
	(void)av;

	// signal_handlers_setup();
	update = (t_ms_state *)malloc(sizeof(t_ms_state));
	if (update == NULL)
	{
		perror ("init_env_copy");
		return (1);
	}
	update->exit_status = 0;
	env_dup = init_env_copy(env);
	if (env_dup == NULL)
	{
		perror ("init_env_copy");
		free(update);
		return (1);
	}
	cmds = (t_cmd **)malloc(sizeof(t_cmd *));
	*cmds = NULL;
	while (1)
	{
		line = readline(C "shell@st42:$ " RST);
		if (line == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO); // Handle Ctrl + D
			rl_clear_history();
			break ; // exit if EOF or error, can be Ctrl + D
		}
		if (line && *line)
		{
			add_history(line);
			if (lexer_and_parse(cmds, line, env_dup, update))
			{
				free_cmds(cmds);
				free(line); //prob dont need to free, only in exit
				continue;
			}
			free(line);
			current = *cmds;
			while(current)
			{
				if (current->builtin)
					execute_builtins(cmds, current->cmd_arr, env_dup, update);
				current = current->next;
			}
			free_cmds(cmds);
		}
	}
	rl_clear_history();
	return (update->exit_status);
}