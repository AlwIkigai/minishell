/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 19:08:20 by asyed             #+#    #+#             */
/*   Updated: 2024/07/04 14:39:37 by ting             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_current_export(t_env *tmp, char *key, char *value)
{
	free(tmp->value);
	tmp->value = value;
	free(key);
}

int	export_error_check(char *args)

{
	int	i;

	if (args == NULL || args[0] == '\0')
		return (1);
	if (args[0] == '=' || ((isalpha(args[0]) == 0) && args[0] != '_'))
	{
		print_error("Not a valid identifier");
		return (-1);
	}
	i = 1;
	while (args[i])
	{
		if (args[i] == '=')
			return (i + 1);
		if ((isalnum(args[i]) == 0) && args[i] != '_')
		{
			print_error("Not a valid identifier");
			return (-1);
		}
		i++;
	}
	return (0);
}

void	assign_key_value(char *args, int equal, char **key, char **value)
{
	if (equal > 0)
	{
		*key = ft_strndup(args, equal - 1);
		*value = ft_strdup(&args[equal]);
	}
	else
	{
		*key = NULL;
		*value = NULL;
	}
}

int	handle_export(char *args, t_env **env_list)
{
	char	*key;
	char	*value;
	t_env	*tmp;
	int		equal;

	equal = export_error_check(args);
	if (equal <= 0)
		return (1);
	assign_key_value(args, equal, &key, &value);
	if (key == NULL && value == NULL)
		return (0);
	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			update_current_export(tmp, key, value);
			return (0);
		}
		tmp = tmp->next;
	}
	if (tmp == NULL)
		ft_add_env_back_node(env_list, ft_new_env_node(key, value));
	return (0);
}

int	builtin_export(char **args, t_env **env_list, int exit_status)
{
	int	i;

	i = 1;
	while (args[i])
	{
		exit_status = handle_export(args[i], env_list);
		i++;
	}
	return (exit_status);
}
