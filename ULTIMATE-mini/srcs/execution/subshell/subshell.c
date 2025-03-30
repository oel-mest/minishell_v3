/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:58:19 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/25 20:31:26 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/execution.h"

/**
 * Executes a local executable file if it exists and is executable.
 * Otherwise, prints the correct Bash-style error message.
 */
void	execute_local_executable(char *cmd, char **args, char **envp)
{
	if (access(cmd, F_OK) != 0)
	{
		print_error(cmd, ": No such file or directory", NULL);
		exit(127);
	}
	if (access(cmd, X_OK) != 0)
	{
		print_error(cmd, ": Permission denied", NULL);
		exit(126);
	}
	execve(cmd, args, envp);
	print_error("execve", NULL);
	exit(126);
}

static void	execute_subshell_child(t_ast *node, t_envnode **envp)
{
	if (node->redi)
	{
		if (node->redi->input)
		{
			if (redirect_input(node->redi->input) == 1)
				exit(1);
		}
		if (node->redi->output)
		{
			if (redirect_output(node->redi->output, node->redi->append) == 1)
				exit(1);
		}
		if (node->redi->heredoc)
			handle_heredoc_if_needed(node->cmd);
	}
	exit(execute_ast(node->left, envp));
}

int	execute_subshell(t_ast *node, t_envnode **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork", NULL);
		return (1);
	}
	if (pid == 0)
		execute_subshell_child(node, envp);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
