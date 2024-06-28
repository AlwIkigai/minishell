/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 12:21:41 by ting              #+#    #+#             */
/*   Updated: 2024/06/28 12:15:47 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

void	free_env(t_env **env_list)
{
	t_env	*tmp;
	t_env	*next;

	if (env_list == NULL || *env_list == NULL)
		return ;
	tmp = *env_list;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->key)
		{
			free(tmp->key);
			tmp->key = NULL;
		}
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		free(tmp);
		tmp = next;
	}
	*env_list = NULL;
}

void	free_lexer(t_lexer **lexer)
{
	t_lexer	*current;
	t_lexer	*next;

	if (!lexer || !*lexer)
		return ;
	current = *lexer;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	*lexer = NULL;
}

void	free_cmds(t_cmd **cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!cmds || !*cmds)
		return ;
	current = *cmds;
	while (current)
	{
		next = current->next;
		free_array(current->cmd_arr);
		if (current->hdoc_delimeter)
			free(current->hdoc_delimeter);
		free_array(current->infile);
		free_array(current->outfile);
		free_array(current->append_re);
		free(current);
		current = next;
	}
	*cmds = NULL;
}

void	free_all_and_exit(t_cmd **cmds, t_env **env_dup, t_ms_state *status)
{
	free_cmds(cmds);
	if (cmds)
		free(cmds);
	free_env(env_dup);
	if (env_dup)
		free(env_dup);
	if (status != NULL)
	{
		free(status);
		status = NULL;
	}
}
