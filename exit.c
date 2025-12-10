/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:53:56 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 19:04:05 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Waits for any potential children
static void	free_arrcmd(t_cmd *arrcmd, int numcmd)
{
	int	i;

	i = 0;
	while (i < numcmd)
	{
		if (arrcmd[i].pid != -1)
			waitpid(arrcmd[i].pid, NULL, 0);
		if (arrcmd[i].argv)
			free_argv(arrcmd[i].argv);
		if (arrcmd[i].pathname)
			free(arrcmd[i].pathname);
		i++;
	}
	free(arrcmd);
}

// We only exit_prog at times where the only remaining open fd
//	is d.stdin_next (read end of the most recently opened pipe)
// Everything else is closed sequentially
void	exit_pipex(t_pipex d, int exitval)
{
	safe_close(&(d.stdin_next));
	safe_close(&(d.dev_null));
	if (d.arrcmd)
		free_arrcmd(d.arrcmd, d.numcmd);
	exit(exitval);
}
