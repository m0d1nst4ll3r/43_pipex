/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:08:54 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/11 21:08:54 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	heredoc_write(int fd, t_heredoc *input)
{
	while (input)
	{
		if (ft_write(fd, input->line, (int)input->len))
			return (1);
		input = input->next;
	}
	return (0);
}

int	heredoc_file(t_heredoc *input)
{
	int	fd;

	fd = open(HERED_FILE, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd == -1)
		return (-1);
	if (heredoc_write(fd, input))
		return (-1);
	close(fd);
	clear_input(input);
	fd = open(HERED_FILE, O_RDONLY);
	unlink(HERED_FILE);
	return (fd);
}

int	heredoc_pipe(t_heredoc *input)
{
	int	pipefd[2];

	if (safe_pipe(pipefd))
		return (-1);
	if (heredoc_write(pipefd[1], input))
		return (-1);
	close(pipefd[1]);
	clear_input(input);
	return (pipefd[0]);
}

int	get_input(t_heredoc **begin, char *limiter)
{
	char		*line;
	size_t		limiter_len;
	t_heredoc	*cur;

	cur = NULL;
	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			continue ;
		if ((!cur || cur->line[cur->len - 1] == '\n')
			&& !strncmp(line, limiter, limiter_len)
			&& (!line[limiter_len] || line[limiter_len] == '\n'))
		{
			free(line);
			return (0);
		}
		cur = add_line(begin, cur, line);
		if (!cur)
			return (1);
	}
}

size_t	get_input_len(t_heredoc *input)
{
	size_t	len;

	len = 0;
	while (input)
	{
		len += input->len;
		input = input->next;
	}
	return (len);
}
