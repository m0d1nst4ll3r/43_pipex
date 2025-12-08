/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:18:30 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 20:32:46 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Only calls close if fd is valid
// Sets fd to -1 after closing
void	safe_close(int &fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

// Nothing really safer about this
// The name just fits with the rest
int	safe_pipe(int fd[2])
{
	if (pipe(pipefd))
	{
		ft_perror_syscall(ERRPIPE);
		return (1);
	}
	return (0);
}

// old has to be passed as address since we want to
//	set it to -1 in case of failure
int	safe_dup2(int &old, int new)
{
	if (*old != -1 && new != -1 && dup2(*old, new))
	{
		ft_perror_syscall(ERRDUP);
		safe_close(&old);
		return (1);
	}
	return (0);
}
