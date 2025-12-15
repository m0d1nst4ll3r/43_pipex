/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:46:17 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 20:58:59 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	usage(void)
{
	ft_printf("usage: ./pipex infile cmd1 [cmd2 cmd3...] cmdn outfile\n");
	ft_printf("       ./pipex here_doc LIMITER cmd1 \
[cmd2 cmd3...] cmdn outfile\n");
	return (0);
}
