/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:00:21 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/25 23:09:29 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"
#include "../../../../includes/execution.h"

// exit (exit shell) with a exit status or exit with 
// the one from the last command or set status to 1 if 
// give more than 1 argument
static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
	{
		count++;
	}
	return (count);
}

static void	process_exit_two_args(char **args)
{
	int	error;
	int	status;

	error = 0;
	status = ft_atoi2(args[1], &error);
	if (error == 1)
	{
		print_error("exit: ", args[1], ": numeric argument required", NULL);
		exit(255);
	}
	exit(status);
}

void	free_ctx(t_exec_ctx *ctx)
{
	if (ctx->args)
		free_split(ctx->args);
	if (ctx->path)
		free(ctx->path);
	if (ctx->path)
		free(ctx->path);
	if (ctx->envp_array)
		free_split(ctx->envp_array);
	// if (ctx->cmd->args)
	// 	free(ctx->cmd->args);
	if (ctx->node)
		free_ast(ctx->node);
	if (ctx->envp)
		free_env_list2(ctx->envp);
	// free(ctx);
}

int	builtin_exit(t_exec_ctx *ctx)
{
	int	arg_count;

	printf("exit\n");
	arg_count = count_args(ctx->args);
	if (arg_count == 1)
	{
		free_ctx(ctx);
		exit(get_status());
	}
	else if (arg_count == 2)
	{
		process_exit_two_args(ctx->args);
	}
	else
	{
		print_error("exit: ", "too many arguments", NULL);
		set_status(1);
		return (1);
	}
	return (0);
}
