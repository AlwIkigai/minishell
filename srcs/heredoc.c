/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ting <ting@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:00:15 by ting              #+#    #+#             */
/*   Updated: 2024/06/29 21:43:50 by ting             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*trim_whitespace(char *str)
{
	char	*end;

	while (*str == ' ' || *str == '\n' || *str == '\r')
		str++;
	if (*str == 0)
		return (str);
	end = str + ft_strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\n' || *end == '\r'))
		end--;
	*(end + 1) = '\0';
	return (str);
}

int    write_to_heredoc_file(t_cmd *current, char *expanded_line, int fd)
{
    char	*trimmed_line;

    if (expanded_line[0] != '\0' && ft_strlen(expanded_line) > 1)
        trimmed_line = trim_whitespace(expanded_line);
    else
    {
        trimmed_line = ft_strdup("");
        free(expanded_line);
    }
    if (ft_strcmp(trimmed_line, current->hdoc_delimeter) == 0)
    {
        free(trimmed_line);
        return (1);
    }
    ft_putendl_fd(trimmed_line, fd);
    free(trimmed_line);
    return (0);
}

void	heredoc_loop(t_cmd *current, t_env **env, t_ms_state *stat, int fd)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (g_reset_cancel == 2)
			break ;
		if (!line)
		{
			printf("shell@st42:$ warning: here-document delimited by end-of-file (wanted `%s')\n",
				current->hdoc_delimeter);
			break ;
		}
		expanded_line = env_var_heredoc(line, env, stat);
        free(line);
        if (write_to_heredoc_file(current, expanded_line, fd))
            break;
	}
}

void	here_doc(t_cmd *current, t_env **env, t_ms_state *stat)
{
	int					fd;
	char				*file;
	struct sigaction	old_sa;

	if (!current->hdoc_delimeter)
		return ;
	file = "./heredoc.tmp";
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (heredoc_sig_set_up(&old_sa) != 0)
		return ;
	heredoc_loop(current, env, stat, fd);
	if (sigaction(SIGINT, &old_sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	close(fd);
	if (!current->infile)
		current->infile = ft_calloc(2, sizeof(char *));
	add_to_arr(&(current->infile), file);
}
