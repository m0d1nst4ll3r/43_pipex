/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:02:47 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 21:12:07 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	resolve_stdin(t_pipex *d, int i)
{
	if (!i)
		if (open_infile(*d, &(d->stdin_next)))
			return (0);
	if (safe_dup2(&(d->stdin_next), STDIN_FILENO))
		return (0);
	safe_close(&stdin_next);
	return (1);
}

int	resolve_stdout(t_pipex *d, int i)
{
	int	pipefd[2];
	int	fd;

	if (i != d->ac - 1)
	{
		if (safe_pipe(pipefd))
			return (0);
		d->stdin_next = pipefd[0];
	}
	else
		if (open_outfile(*d, pipefd + 1))
			return (0);
	if (safe_dup2(pipefd + 1, STDOUT_FILENO))
		return (0);
	if (i != d->ac - 1)
		safe_close(pipefd + 1);
	return (1);
}
