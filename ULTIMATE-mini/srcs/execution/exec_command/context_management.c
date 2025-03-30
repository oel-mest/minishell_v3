/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:22:12 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/26 03:16:48 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../includes/execution.h"

static int	handle_no_args(t_exec_ctx *ctx)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (ctx->cmd->output2)
		if (redirect_output(ctx->cmd->output2, ctx->cmd->append))
			return (1);
	if (ctx->cmd->input2)
	{
		if (redirect_input(ctx->cmd->input2))
			return (1);
	}
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	return (1);
}

static int	expand_and_check_args(t_exec_ctx *ctx, t_envnode **envp)
{
	ctx->args = expand_env_vars2(ctx->args, *envp);
	char *tmp = ctx->cmd->args;
	ctx->cmd->args = expand_env_vars(ctx->cmd->args, *envp);
	free(tmp);
	if (!ctx->args)
		return (handle_no_args(ctx));
	if (ctx->args[0] == NULL)
		return (1);
	hundle_awk(ctx->args);
	ctx->args[0] = remove_first_layer_quotes(ctx->args[0]); 
	return (0);
}

static int	exec_builtin(t_exec_ctx *ctx)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (ctx->cmd->output2)
	{
		if (redirect_output(ctx->cmd->output2, ctx->cmd->append))
			return (1);
	}
	if (ctx->cmd->input2)
	{
		if (redirect_input(ctx->cmd->input2))
			return (1);
	}
	ret = run_builtin(ctx);
	set_status(ret);
	free_withoutexit(ctx);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	return (ret);
}

int	setup_context(t_exec_ctx *ctx, t_ast *tree, t_envnode **envp)
{
	ctx->cmd = tree->cmd;
	ctx->envp = envp;
	ctx->args = NULL;
	ctx->path = NULL;
	ctx->envp_array = NULL;
	ctx->node = tree;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	ctx->cmd->args = expand_line(ctx->cmd->args);
	ctx->args = ft_split(ctx->cmd->args, ' ');
	if (expand_and_check_args(ctx, envp) != 0)
		return (1);
	if (ctx->args[0] && is_builtin(ctx->args[0]))
	{
		exec_builtin(ctx);
		return (5);
	}
	if (check_if_folder(ctx->args[0]) == 1)
	{
		if (ctx->args)
			free(ctx->args);
		return (126);
	}
	return (0);
}
