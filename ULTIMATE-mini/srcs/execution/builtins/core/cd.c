/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:57:46 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/26 03:01:30 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"
#include "../../../../includes/execution.h"

static int	cd_home(char **args, t_envnode **envp)
{
	t_envnode	*node;
	char		*home;

	node = find_env_var(*envp, "HOME");
	if (!node)
	{
		print_error("cd: HOME not set", NULL);
		set_status(1);
		return (1);
	}
	home = ft_strdup(node->value);
	if (home)
		hundle(&home);
	if (chdir(home) != 0)
	{
		print_error("cd: ", home, ": No such file or directory", NULL);
		set_status(1);
		return (1);
	}
	free(home);
	return (0);
}

static int	cd_oldpwd(char **args, t_envnode **envp)
{
	t_envnode	*node;
	char		*oldpwd;

	node = find_env_var(*envp, "OLDPWD");
	if (!node)
	{
		print_error("bash: cd: OLDPWD not set", NULL);
		set_status(1);
		return (1);
	}
	oldpwd = ft_strdup(node->value);
	if (chdir(oldpwd) != 0)
	{
		print_error("bash: cd: ", oldpwd, ": No such file or directory", NULL);
		set_status(1);
		return (1);
	}
	free(oldpwd);
	return (0);
}

int	builtin_cd(char **args, t_envnode **envp)
{
	char	*oldpwd;
	char	*join;
	int		i;
	int		ret;

	ret = 0;
	oldpwd = getcwd(NULL, 0);
	if (!args[1] || !strcmp(args[1], "~"))
		ret = cd_home(args, envp);
	else if (!strcmp(args[1], "-"))
		ret = cd_oldpwd(args, envp);
	else if (chdir(args[1]) != 0)
	{
		print_error("bash: cd: ", args[1], ": No such file or directory", NULL);
		set_status(1);
		return (1);
	}
	set_pwd_oldpwd(envp, oldpwd);
	return (ret);
}
