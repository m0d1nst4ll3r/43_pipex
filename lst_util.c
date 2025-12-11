/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:52:46 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/11 21:02:02 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clear_input(t_heredoc *input)
{
	t_heredoc	*last;

	while (input)
	{
		last = input;
		input = input->next;
		free(last->line);
		free(last);
	}
}

t_heredoc	*add_line(t_heredoc **begin, t_heredoc *cur, char *line)
{
	t_heredoc	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->line = line;
	new->len = ft_strlen(line);
	new->next = NULL;
	if (!*begin)
		*begin = new;
	else
		cur->next = new;
	return (new);
}
