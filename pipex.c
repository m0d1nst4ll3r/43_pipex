/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 14:30:39 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 20:16:41 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// d short for data (since we will be writing it a lot)
void	pipex(t_pipex d)
{
	int	i;

	i = 0;
	while (i < d.numcmd)
	{
		if (resolve_stdin(&d, i) && resolve_stdout(&d, i)
			&& resolve_argv(d, i) && resolve_pathname(d, i))
		{
			d.arrcmd[i].pid = fork();
			if (d.arrcmd[i].pid == -1)
				ft_perror_syscall(ERRFORK);
			else if (!d.arrcmd[i].pid)
			{
				safe_close(&(d.dev_null));
				if (i + 1 < d.numcmd)
					close(d.stdin_next);
				if (execve(d.arrcmd[i].pathname, d.arrcmd[i].argv, d.ep) == -1)
				{
					ft_perror_syscall(ERREXEC);
					exit(1);
				}
			}
		}
		i++;
	}
}
