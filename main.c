/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:09:01 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 20:56:17 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// See program steps in header file
int	main(int ac, char **av, char **ep)
{
	t_pipex	data;

	if (ac < 5)
		return (usage());
	data.heredoc = 0;
	if (!ft_strcmp(av[1], HEREDOC))
		data.heredoc = 1;
	if (data.heredoc && ac < 6)
		return (usage());
	init_pipex(ac, av, ep, &data);
	pipex(data);
	exit_pipex(data);
}
