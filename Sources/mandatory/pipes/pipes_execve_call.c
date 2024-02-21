/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execve_call.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:43:04 by abourgeo          #+#    #+#             */
/*   Updated: 2024/02/21 08:19:53 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

/**
 * If a single command was called (and is not a builtin) we will then 
 * execute this function which is supposed to create arguements needed by 
 * execve. On succes, exits with 0. On error, exits with a non-zero error code.
 * @param exec_struct The data of our program.
 * @param table The data generated by the input.
*/
int	single_process(t_exec *exec_struct, t_table *table, char *input)
{
	char	*path_cmd;
	char	**args_cmd;
	char	**env;
	int		redir;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	free(input);
	redir = redirections(table);
	if (redir == 0 || is_a_directory(table->cmd) == 1)
	{
		free_process(*exec_struct, NULL, NULL, NULL);
		exit(1 + (125 * redir));
	}
	path_cmd = find_path(exec_struct->shell->env, table->cmd);
	if (path_cmd == NULL)
		path_cmd_not_found(exec_struct, table);
	args_cmd = get_args_cmd(table->args);
	env = copy_env(exec_struct->shell->env);
	free_process(*exec_struct, NULL, NULL, NULL);
	execve(path_cmd, args_cmd, env);
	return (execve_fail(path_cmd, args_cmd, env));
}

int	command_in_pipe(t_pipex *pipex, t_table *table)
{
	char	*path_cmd;
	char	**args_cmd;
	char	**env;

	path_cmd = find_path(pipex->exec_struct->shell->env, table->cmd);
	if (path_cmd == NULL)
	{
		write(2, table->cmd, ft_strlen(table->cmd));
		write(2, ": Command not found\n", 20);
		exit_child_process(pipex, 127);
	}
	args_cmd = get_args_cmd(table->args);
	env = copy_env(pipex->exec_struct->shell->env);
	exit_child_process(pipex, -1);
	execve(path_cmd, args_cmd, env);
	perror(path_cmd);
	free(path_cmd);
	free_tabtab(args_cmd);
	free_tabtab(env);
	exit(errno);
}

int	builtin_in_pipe(t_pipex *pipex, t_table *table_cmd)
{
	int	status;

	status = builtin_execution(pipex->exec_struct, table_cmd);
	exit_child_process(pipex, -1);
	exit(status);
}
