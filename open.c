/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:00:57 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 20:40:28 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// TODO TODO TODO TODO TODO TODO TODO TODO TODO HEREDOC
int	open_infile(t_pipex d, int *fd)
{
	if (d.heredoc)
	{
		*fd = get_heredoc(/*TODO*/);
		if (!*fd)
		{
			ft_perror(HEREDOC);
			return (1);
		}
	}
	else
	{
		*fd = open(d.av[2], O_RDONLY);
		if (!*fd)
		{
			ft_perror(d.av[2]);
			return (1);
		}
	}
	return (0);
}

int	open_outfile(t_pipex d, int *fd)
{
	*fd = open(d.av[d.ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!*fd)
	{
		ft_perror(d.av[d.ac - 1]);
		return (1);
	}
	return (0);
}

