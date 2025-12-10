/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:49:06 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 16:05:21 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Ex print:
//	zsh: no such file or directory: file
void	ft_perror(char *s)
{
	ft_fprintf(2, SH_NAME ": %s: %s\n", strerror(errno), s);
}

// Ex print:
//	zsh: pipe failure: The per-process limit on the number of open file
//	descriptors has been reached.
void	ft_perror_syscall(char *s)
{
	ft_fprintf(2, SH_NAME ": %s: %s\n", s, strerror(errno));
}

// Ex print:
//	zsh: command not found: abc
void	ft_perror_path(char *s)
{
	ft_fprintf(2, SH_NAME ": %s: %s\n", ERRPATH, s);
}

void	critical_error(t_pipex d, char *s)
{
	ft_perror_syscall(s);
	ft_fprintf(2, "Aborting...\n");
	exit_pipex(d, 1);
}
