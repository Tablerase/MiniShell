/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:55:44 by abourgeo          #+#    #+#             */
/*   Updated: 2024/02/20 10:56:28 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"

/**
 * Returns the current value of the PATH environment variable 
 * (if it has not been unseted).
 * @param env_list The list of environment variables of our program.
 * @param cmd The command we want to find.
*/
char	*find_path(t_env_list **env_list, char *cmd)
{
	char	*path;

	if (cmd == NULL)
		return (NULL);
	if (cmd[0] == '/')
		return (ft_strdup(cmd));
	path = ft_getenv("PATH", env_list);
	if (path == NULL)
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = search_path(path, cmd);
	return (path);
}

/**
 * Searches the absolute path of the command we are willing to execute.
 * If it is found, this path is returned. Else NULL is returned.
 * @param path The value of the PATH environment variable.
 * @param cmd The command we are searching for.
*/
char	*search_path(char *path, char *cmd)
{
	int		i;
	char	**split_path;

	i = 0;
	split_path = ft_split(path, ':');
	if (split_path == NULL)
		return (NULL);
	while (split_path[i] != NULL)
	{
		path = try_path(split_path[i], cmd);
		if (path != NULL)
		{
			if (access(path, F_OK) == 0)
				return (free_tabtab(split_path), path);
			free(path);
		}
		i++;
	}
	free_tabtab(split_path);
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

/**
 * Builts the absolute path with a part of PATH.
 * @param first_part The first part of the absolute path.
 * @param cmd The command we will add at the end of our string.
*/
char	*try_path(char *first_part, char *cmd)
{
	char	*tmp_path;
	char	*path;

	tmp_path = ft_strjoin(first_part, "/");
	if (tmp_path == NULL)
		return (NULL);
	path = ft_strjoin(tmp_path, cmd);
	free(tmp_path);
	if (path == NULL)
		return (NULL);
	return (path);
}

/**
 * Calculates the size of the given list.
 * @param list The list we want the size of.
*/
int	size_list(t_env_list **list)
{
	int			len;
	t_env_list	*tmp;

	len = 0;
	if (list == NULL)
		return (0);
	tmp = *list;
	while (tmp != NULL)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

int	command_not_found(t_exec *exec_struct, t_table *table, char *path_cmd)
{
	write(2, table->cmd, ft_strlen(table->cmd));
	write(2, ": Command not found\n", 20);
	free_process(*exec_struct, path_cmd, NULL, NULL);
	exit(127);
}
