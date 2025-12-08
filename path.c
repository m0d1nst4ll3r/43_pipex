/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:41:10 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 20:03:09 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

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

static int	test_path(char *path, char *bin, char **pathname)
{
	char	*pathname;
	size_t	bin_len;

	bin_len = ft_strlen(bin);
	*pathname = build_pathname(path, bin, bin_len);
	if (!*pathname)
		return (-1);
	if (!access(*pathname, X_OK))
		return (1);
	free(*pathname);
	return (0);
}

// Sets valid pathname for bin or NULL
// Returns -1 in case of malloc error, 1 for success
int	get_bin_path(char *env_path, char *bin, char **bin_path)
{
	int		i;
	int		ret;

	i = 0;
	while (env_path[i])
	{
		if (env_path[i] != ':')
		{
			ret = test_path(env_path + i, bin, &bin_path);
			if (ret == -1)
				return (-1);
			else if (ret)
				return (1);
			while (env_path[i] && env_path[i] != ':')
				i++;
		}
		else
			i++;
	}
	return (NULL);
}

char	*get_path_from_env(char **ep)
{
	int	i;

	i = 0;
	while (ep[i])
	{
		if (!ft_strncmp(ep[i], "PATH="))
			return (ep[i] + 5);
		i++;
	}
	return (NULL);
}
