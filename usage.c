/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:46:17 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 15:02:28 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	usage(void)
{
	ft_printf("usage: ./pipex file1 cmd1 [cmd2 cmd3...] cmdn file2\n");
	return (0);
}

int	usage_heredoc(void)
{
	ft_printf("usage: ./pipex here_doc LIMITER cmd1 [cmd2 cmd3...] cmdn file\n");
	return (0);
}
