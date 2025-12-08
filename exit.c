/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:53:56 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 21:22:31 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

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
}

// We only exit_prog at times where the only remaining open fd
//	is d.stdin_next (read end of the most recently opened pipe)
// Everything else is closed sequentially
void	exit_prog(t_pipex d)
{
	safe_close(&d.stdin_next);
	if (d.arrcmd)
		free_arrcmd(d.arrcmd, d.numcmd);
	exit(1);
}
