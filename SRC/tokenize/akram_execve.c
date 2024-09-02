/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   akram_execve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 20:44:40 by mbamatra          #+#    #+#             */
/*   Updated: 2024/09/02 08:52:39 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

///////i want to do the pipes here by doing all the commands with each other and then waiting for the all
///////untill they finish
////// so ill do a while the while loop here and also you need one fork for each command
#include "../../Includes/minishell.h"

void	akrm(t_cmd *cmd, char **envp, t_data *data ,int *tmd_fd)
{
	char **path;
	char *tmp_cmd;
	int	pip[2];
	t_cmd *full_cmd;

	full_cmd = cmd;
	while (cmd)
	{
		pipe(pip);
		cmd->pid = fork();
		if (cmd->pid == -1)
			exit(1);
		if (cmd->pid == 0)
		{
			close(tmd_fd[0]);
			close(tmd_fd[1]);
			close(pip[0]);
			if (cmd->next)
				dup2(pip[1], 1);
			close(pip[1]);
			handle_redirection(cmd->files);
			if (!cmd->content || !cmd->content[0])
				exit(0);
			if (execve(cmd->content[0], cmd->content, envp) == -1) /// /bin/ls
			{
				path = get_path(data->envp_list);
				if (path)
				{
					tmp_cmd = cmd->content[0];
					while (*path)
					{
						tmp_cmd = ft_strjoin(ft_strjoin(*path, "/"), tmp_cmd);
						if (execve(tmp_cmd, cmd->content, envp) != -1)
							break;
						free(tmp_cmd);
						tmp_cmd = cmd->content[0];
						path++;
					}
				}
				write(2, cmd->content[0], ft_strlen(cmd->content[0]));
				write(2, ": command not found\n", 21);
				exit(127);
			}
		}
		else
		{
			close(pip[1]);
			dup2(pip[0], 0);
			close(pip[0]);
			cmd = cmd->next;
		}
	}
	while (full_cmd)
	{
		waitpid(full_cmd->pid, &data->exit_status, 0);
		full_cmd = full_cmd->next;
	}
}
