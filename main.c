/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:09:01 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/11 12:42:45 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// See program steps in header file
int	main(int ac, char **av, char **ep)
{
	t_pipex	data;

	if (ac < 5)
		return (usage());
	init_pipex(ac, av, ep, &data);
	if (data.heredoc && ac < 6)
		return (usage_heredoc());
	pipex(data);
	exit_pipex(data, 0);
}
