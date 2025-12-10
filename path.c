/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:41:10 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 18:03:14 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*build_pathname(char *path, char *bin, size_t bin_len)
{
	char	*pathname;
	size_t	len;

	len = 0;
	while (path[len] && path[len] != ':')
		len++;
	pathname = malloc(len + bin_len + 2);
	if (!pathname)
		return (NULL);
	ft_strlcpy(pathname, path, len + 1);
	ft_strcat(pathname, "/");
	ft_strcat(pathname, bin);
	return (pathname);
}

// Need to differentiate between 3 states
// 1. Success (found pathname)
// 2. Failure (no valid pathname)
// 3. Malloc error
// As such we can't directly return pathname, we use an address
static int	test_path(char *path, char *bin, size_t bin_len, char **pathname)
{
	*pathname = build_pathname(path, bin, bin_len);
	if (!*pathname)
		return (-1);
	if (!access(*pathname, X_OK))
		return (1);
	free(*pathname);
	*pathname = NULL;
	return (0);
}

// Need to pass pathname by address for malloc errors in test_path()
int	get_bin_path(char *env_path, char *bin, char **pathname)
{
	int		i;
	int		ret;
	size_t	bin_len;

	if (!env_path || !bin)
		return (0);
	bin_len = ft_strlen(bin);
	i = 0;
	while (env_path[i])
	{
		if (env_path[i] != ':')
		{
			ret = test_path(env_path + i, bin, bin_len, pathname);
			if (ret)
				return (ret);
			while (env_path[i] && env_path[i] != ':')
				i++;
		}
		else
			i++;
	}
	return (0);
}

// If cmd was totally empty (argv is empty), give an error
// In case of error, directly free argv since we don't need it anymore
int	resolve_pathname(t_pipex d, int i)
{
	int		ret;

	ret = get_bin_path(d.path, d.arrcmd[i].argv[0], &(d.arrcmd[i].pathname));
	if (!ret)
	{
		if (d.arrcmd[i].argv[0])
			ft_perror_path(d.arrcmd[i].argv[0]);
		else
			ft_perror_path("");
		free_argv(d.arrcmd[i].argv);
		d.arrcmd[i].argv = NULL;
		return (0);
	}
	else if (ret == -1)
		critical_error(d, ERRMALLOC);
	return (1);
}
