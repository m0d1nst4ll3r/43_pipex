/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:00:57 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/11 12:46:51 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Try to only open once
int	open_devnull(t_pipex *d, int *fd)
{
	if (d->dev_null == -1)
		d->dev_null = open(DEVNULL, O_RDONLY);
	if (d->dev_null == -1)
	{
		ft_perror(DEVNULL);
		return (1);
	}
	*fd = d->dev_null;
	return (0);
}

// TODO TODO TODO TODO TODO TODO TODO TODO TODO HEREDOC
int	open_infile(t_pipex d, int *fd)
{
	if (d.heredoc)
	{
		*fd = 0;//get_heredoc(/*TODO*/);
		if (*fd == -1)
		{
			ft_perror(HEREDOC);
			return (1);
		}
	}
	else
	{
		*fd = open(d.av[1], O_RDONLY);
		if (*fd == -1)
		{
			ft_perror(d.av[1]);
			return (1);
		}
	}
	return (0);
}

int	open_outfile(t_pipex d, int *fd)
{
	if (d.heredoc)
		*fd = open(d.av[d.ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd = open(d.av[d.ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		ft_perror(d.av[d.ac - 1]);
		return (1);
	}
	return (0);
}

