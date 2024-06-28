/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 15:10:54 by ting              #+#    #+#             */
/*   Updated: 2024/06/28 17:09:26 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern volatile sig_atomic_t	g_reset_cancel;

int	illegal_builtins(t_cmd *current)
{
	if (current->builtin && (!ft_strcmp(current->cmd_arr[0], "cd")
			|| !ft_strcmp(current->cmd_arr[0], "export")
			|| !ft_strcmp(current->cmd_arr[0], "unset")
			|| !ft_strcmp(current->cmd_arr[0], "exit")))
	{
		return (1);
	}
	return (0);
}

void	free_n_exit_child(t_pipeline *pipeline)
{
	int	stat;
	int	num_cmds;

	num_cmds = cmds_len((*pipeline->cmds));
	stat = pipeline->status->exit_status;
	free_all_and_exit(pipeline->cmds, pipeline->env, pipeline->status);
	free_pipe_ends(pipeline->pipe_ends, num_cmds);
	free(pipeline);
	exit(stat);
}

void	execute_cmd(t_cmd *cmd, t_env **env, t_ms_state *status)
{
	char	*cmd_path;

	cmd_path = find_path(cmd->cmd_arr[0], env);
	if (cmd_path)
	{
		execve(cmd_path, cmd->cmd_arr, env_in_arr(env));
		free(cmd_path);
	}
	else
	{
		printf("%s: command not found\n", cmd->cmd_arr[0]);
		status->exit_status = 127;
	}
}

// void	here_doc(t_cmd *current)
// {
// 	char	*line;
// 	int		fd;
// 	char	*file;
	
// 	if (!current->hdoc_delimeter)
// 		return ;
// 	file = "./heredoc.tmp";
// 	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0777);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			printf("shell@st42:$ warning: here-document delimited by end-of-file (wanted `%s')\n", current->hdoc_delimeter);
// 			break;
// 		}
// 		if (ft_strcmp(line, current->hdoc_delimeter) == 0)
//         {
//             free(line);
//             break;
//         }
// 		ft_putendl_fd(line, fd);
// 		free(line);
// 	}
// 	close(fd);
// 	if (!current->infile)
//         current->infile = ft_calloc(2, sizeof(char *));
// 	add_to_arr(&(current->infile), file);
// }

// void	ctrl_c_heredoc(int sig)
// {
// 	t_ms_state	*status;

// 	status = *set_stats();
// 	if (sig == SIGINT)
// 	{
// 		status->exit_status = 130;
// 		g_reset_cancel = 1;  // Set the flag to indicate Ctrl-C was pressed
// 		rl_done = 1;  // This tells readline to return immediately
// 	}
// }


char	*trim_whitespace(char *str)
{
	char	*end;

	// Trim leading space
	while (*str == ' ' || *str == '\n' || *str == '\r')
		str++;

	if (*str == 0)
		return (str);

	// Trim trailing space
	end = str + ft_strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\n' || *end == '\r'))
		end--;

	// Write new null terminator
	*(end + 1) = '\0';

	return (str);
}

void	here_doc_handler(int siggy)
{
	(void)siggy;
	g_reset_cancel = 2;
    if (isatty(STDIN_FILENO))
        ft_putstr_fd("\n", STDOUT_FILENO);
}

int	here_doc_set_up(struct sigaction *old_sa)

{
	struct sigaction	here_doc;
	
	here_doc.sa_handler = here_doc_handler;
	sigemptyset(&here_doc.sa_mask);
	here_doc.sa_flags = 0;
	if (sigaction(SIGINT, &here_doc, old_sa) == -1)
	{
		perror("here_doc_ctrl_c");
		exit (EXIT_FAILURE);
	}
	return (0);
}

void	here_doc(t_cmd *current)
{
	char	*line;
	int		fd;
	char	*file;
	struct sigaction old_sa;
	if (!current->hdoc_delimeter)
		return;
	file = "./heredoc.tmp";
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (here_doc_set_up(&old_sa) != 0)
		return;
	while (1)
	{
		if (g_reset_cancel == 2)
			break;
		write(1, "> ", 2);
		line = get_next_line(0);  // 0 is the file descriptor for standard input
		if (g_reset_cancel == 2)
		{
			free (line);
			break;
		}
		if (!line)
		{
			printf("shell@st42:$ warning: here-document delimited by end-of-file (wanted `%s')\n", current->hdoc_delimeter);
			break;
		}
		char *trimmed_line = trim_whitespace(line);
		if (ft_strcmp(trimmed_line, current->hdoc_delimeter) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
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

void	do_single_cmd(t_cmd **cmds, t_env **env, t_ms_state *status)
{
	int	pid;
	int	stat;
	int	exit_status;

	if (illegal_builtins((*cmds)))
		return (execute_builtins(cmds, (*cmds)->cmd_arr, env, status));
	here_doc((*cmds));
	pid = fork();
	if (pid < 0)
		perror("fork error");
	if (pid == 0)
	{
		do_redirection((*cmds), status);
		if ((*cmds)->builtin)
			execute_builtins(cmds, (*cmds)->cmd_arr, env, status);
		else if ((*cmds)->cmd_arr[0])
			execute_cmd((*cmds), env, status);
		stat = status->exit_status;
		free_all_and_exit(cmds, env, status);
		exit(stat);
	}
	wait(&exit_status);
	if (WIFEXITED(exit_status))
		status->exit_status = WEXITSTATUS(exit_status);
}



void	execute_child_process(t_pipeline *pipeline, t_cmd *current, int i)
{
	init_dup(pipeline->num_cmds, i, pipeline->pipe_ends);
	do_redirection(current, pipeline->status);
	if (illegal_builtins(current))
		free_n_exit_child(pipeline);
	if (current->builtin)
		execute_builtins(&current, current->cmd_arr, pipeline->env,
			pipeline->status);
	else if (current->cmd_arr[0])
		execute_cmd(current, pipeline->env, pipeline->status);
	free_n_exit_child(pipeline);
}
