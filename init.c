/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:51:12 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 21:18:23 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

// Init pid etc... since we might skip initializing them
// E.g when a command is skipped due to an error
// We still parse this at program end (or critical failure)
//	to wait for every pid/free all
void	init_arrcmd(t_cmd *arrcmd, int numcmd)
{
	int	i;

	i = 0;
	while (i < numcmd)
	{
		arrcmd[i].pid = -1;
		arrcmd[i].pathname = NULL;
		arrcmd[i].argv = NULL;
		i++;
	}
}

void	init_data(int ac, char **av, char **ep, t_pipex *d)
{
	d->ac = ac;
	d->av = av;
	d->ep = ep;
	d->path = get_path_from_env(ep);
	d->stdin_next = -1;
	d->heredoc = 0;
	if (!ft_strcmp(av[1], HEREDOC))
		d->heredoc = 1;
	d->numcmd = ac - 2;
	if (d->heredoc)
		d->numcmd = ac - 3;
	d->arrcmd = malloc(sizeof(*(d.arrcmd)) * numcmd);
	if (!d->arrcmd)
		critical_error(ERRMALLOC);
	init_arrcmd(d->arrcmd, d->numcmd);
}
