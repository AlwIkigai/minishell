/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:12:23 by ting              #+#    #+#             */
/*   Updated: 2024/06/28 13:39:34 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>  //for history handling
# include <readline/readline.h> //for command line input
# include <stdio.h>
// for standard input/output and memory management
# include <stdlib.h>
// for standard input/output and memory management
# include <fcntl.h>
# include <signal.h>    //for sigaction
# include <string.h>    //for string manipulation
# include <sys/types.h> //for process management
# include <sys/wait.h>  //for process management
# include <unistd.h>    //POSIX API functions

# define G "\001\033[32m\002"
# define M "\001033[35m\002"
# define C "\001\033[36m\002"
# define RST "\001\033[0m\002"
# define MALLOC_ERROR 1

extern volatile sig_atomic_t	g_reset_cancel;

typedef struct s_ms_state
{
	int				exit_status;
}					t_ms_state;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_lexer
{
	char *str; // have to free
	int type;  // word(1), '|'(2), '<'(3), '>'(4), '<<'(5), '>>'(6),
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

typedef struct s_cmd
{
	char			**cmd_arr;
	int				in_pid;
	int				out_pid;
	char			**infile;
	char			**outfile;
	int				builtin;
	char			**append_re;
	char			*hdoc_delimeter;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_pipeline
{
	t_cmd			**cmds;
	t_env			**env;
	t_ms_state		*status;
	int				**pipe_ends;
	int				num_cmds;
}					t_pipeline;

//-------------------INITIALISE--------------------

// init_status_envdup.c
t_ms_state			*init_status(void);
t_env				**init_envdup(t_ms_state *status, char **env);

//--------------------SIGNALS----------------------

// signals.c
void				sigexit_handler(t_cmd **cmds, t_env **env_dup,
						t_ms_state *status);
int					signal_handlers_setup(t_ms_state *status);
void				cancel_handler(int siggy);
int					here_doc_set_up(struct sigaction *old_sa);
t_ms_state			**set_stats(void);
void				extract_stats(t_ms_state *extraction);

//--------------------BUILT-INS--------------------

// builtins.c
void				execute_builtins(t_cmd **cmds, char **args, t_env **env_dup,
						t_ms_state *status);

// print_error_msg.c
void				print_error(char *str);

// builtin_echo.c
int					builtin_echo(char **args);

//	builtin_cd_pwd.c
int					builtin_cd(char **args, t_env **env_list);
int					builtin_pwd(void);
void				update_pwd(t_env **env_list);

// builtin_export.c
void				update_current_export(t_env *tmp, char *key, char *value);
int					export_error_check(char *args);
void				assign_key_value(char *args, int equal, char **key,
						char **value);
int					handle_export(char *args, t_env **env_list);
int					builtin_export(char **args, t_env **env_list,
						int exit_status);

// builtin_unset.c
void				free_unset_node(t_env *del);
int					builtin_unset(char **args, t_env **env_list);

// builtin_env.c
char				*find_env(t_env **env_list, char *to_find);
void				create_copy(t_env **env_list, char *env);
int					builtin_env(t_env **env_list, char **args);
void				print_env_var(void *env_list);
t_env				**env_error(char *message);
t_env				**init_env_copy(char **env);

// builtin_exit.c
int					check_negative(char **args);
void				not_numeric(t_cmd **cmds, t_env **env, t_ms_state *status);
int					too_many_args(char **args);
int					confirmed_exit_status(char **args);
int					builtin_exit(t_cmd **cmds, t_env **env, char **args,
						t_ms_state *status);

// builtins_env_node_utils.c
t_env				*ft_new_env_node(char *key, char *value);
void				ft_add_env_back_node(t_env **lst, t_env *new);

//--------------------PARSING--------------------

// lexer.c
int					quotes_token(char *str, int i);
void				handle_special_token(t_lexer **lexer, char *str, int *i);
int					skip_wp(char *str, int *i);
int					tokenizer(t_lexer **lexer, char *str);
int					lexer_and_parse(t_cmd **cmds, char *str, t_env **env_dup,
						t_ms_state *stat);

// lexer_utils.c
t_lexer				*new_lexer(char *str);
void				lexer_add_back(t_lexer **lexer, t_lexer *new);
void				del_lexer(t_lexer **lexer, t_lexer *to_del);
int					is_special_char(char c);
void	print_lexer(t_lexer **lexer); // to delete later

// expand_env_var.c
void				replace_env_var(t_lexer *lexer, int var_start, int var_len,
						char *value);
int					cal_var_len(char *str);
char				*get_env_value(char *var, int *free_flag, t_env **env_dup,
						t_ms_state *stat);
int					expand_env_var(t_lexer *lexer, int i, t_env **env_dup,
						t_ms_state *stat);
void				check_env_var(t_lexer *lexer, t_env **env_dup,
						t_ms_state *stat);

// remove_quotes.c
int					remove_quotes(t_lexer *lexer);

// parsing.c
void				check_builtins(t_cmd **cmds);
int					parsing(t_lexer **lexer, t_cmd **cmds);
int					handle_lexer(t_lexer **curr_l, t_cmd **cmd, char **arr);
int					handle_redirection(t_lexer **curr_l, t_cmd *cmd);
int					handle_append_or_heredoc(t_lexer **curr_l, t_cmd *cmd);

// parsing_utils.c
t_cmd				*new_cmd(char **arr);
void				cmd_add_back(t_cmd **cmds, t_cmd *new);
void				print_parse(t_cmd **cmds);
int					cal_arg_count(t_lexer *curr_l);
int					get_arr_size(char **arr);
void				add_to_arr(char ***arr, char *str);

//--------------------EXECUTION--------------------

//execute_pipeline.c
void				init_dup(int num_cmds, int i, int **pipe_ends);
t_pipeline			*init_pipeline(t_cmd **cmds, t_env **env, t_ms_state *status);
void				parent_wait(t_ms_state *status, int num_cmds);
void				execute_pipeline(t_cmd **cmds, t_env **env, t_ms_state *status);

// execution.c
int					illegal_builtins(t_cmd *current);
void				free_n_exit_child(t_pipeline *pipeline);
void				execute_cmd(t_cmd *cmd, t_env **env, t_ms_state *status);
void				do_single_cmd(t_cmd **cmds, t_env **env,
						t_ms_state *status);
void				execute_child_process(t_pipeline *pipeline, t_cmd *current, int i);
void				here_doc(t_cmd *current);

// execution_utils.c
int					cmds_len(t_cmd *cmds);
char				*join_key_value(const char *key, const char *value);
char				**env_in_arr(t_env **env);
char				*find_path(char *cmd, t_env **env);

// pipe.c
int					**init_pipe_ends(int num_cmds);
void				init_pipes(int **pipe_ends, int num_cmds);
void				free_pipe_ends(int **pipe_ends, int num_cmds);
void				close_pipe_ends(int **pipe_ends, int num_cmds);

// redirections.c
int					is_append_re(char *filename, char **append_re);
int					handle_infile(t_cmd *cmd, t_ms_state *stat);
int					handle_outfile(t_cmd *cmd, t_ms_state *stat);
int					do_redirection(t_cmd *cmd, t_ms_state *stat);

//--------------------FREEING--------------------

// free.c
void				free_array(char **array);
void				free_env(t_env **env_list);
void				free_lexer(t_lexer **lexer);
void				free_cmds(t_cmd **cmds);
void				free_all_and_exit(t_cmd **cmds, t_env **env_dup,
						t_ms_state *status);

#endif
