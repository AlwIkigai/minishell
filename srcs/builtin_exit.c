/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:05:41 by asyed             #+#    #+#             */
/*   Updated: 2024/07/04 14:38:46 by ting             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_negative(char **args)
{
	int	i;

	i = 0;
	if (args[1][0] == '-' && args[1][i + 1])
		i = 1;
	return (i);
}

void	not_numeric(t_cmd **cmds, t_env **env, t_ms_state *status)
{
	int	exit_stats;

	exit_stats = 2;
	print_error("Numeric arguments required");
	status->exit_status = exit_stats;
	rl_clear_history();
	free_all_and_exit(cmds, env, status);
	exit(exit_stats);
}

int	too_many_args(char **args)
{
	int	ac;

	ac = 0;
	while (args[ac])
		ac++;
	if (ac > 2)
	{
		print_error("Too many arguments");
		return (1);
	}
	return (0);
}

int	confirmed_exit_status(char **args)
{
	int	av;
	int	exit_stats;

	if (args[1] != NULL)
	{
		av = ft_atoi(args[1]);
		exit_stats = av & 0xFF;
	}
	else
	{
		exit_stats = 0;
	}
	return (exit_stats);
}

int	builtin_exit(t_cmd **cmds, t_env **env, char **args, t_ms_state *status)
{
	int	exit_stats;
	int	i;

	exit_stats = 0;
	i = 0;
	printf("exit\n");
	if (args[1] != NULL)
	{
		i = check_negative(args);
		while (args[1][i])
		{
			if (ft_isdigit(args[1][i]) == 0)
				not_numeric(cmds, env, status);
			i++;
		}
	}
	if (too_many_args(args))
		return (1);
	if (exit_stats == 0)
		exit_stats = confirmed_exit_status(args);
	status->exit_status = exit_stats;
	rl_clear_history();
	free_all_and_exit(cmds, env, status);
	exit(exit_stats);
}
