/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:47:42 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 16:05:11 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Pretty simple for now, but will get more complicated in V2
// As we need to handle quotes and brackets and maybe more
static char	**get_cmd_argv(char *cmd)
{
	return (ft_split(cmd, ' '));
}

int	resolve_argv(t_pipex d, int i)
{
	d.arrcmd[i].argv = get_cmd_argv(d.av[i + 2 + d.heredoc]);
	if (!d.arrcmd[i].argv)
		critical_error(d, ERRMALLOC);
	return (1);
}
