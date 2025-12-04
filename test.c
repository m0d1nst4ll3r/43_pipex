#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"

/*
// example input
//	/user/bin:[...]
//	ls
// example output
//	/user/bin/ls
char	*create_path(char *path, char *bin, int bin_len)
{
	int		len;
	char	*new;

	len = 0;
	while (path[len] && path[len] != ':')
		len++;
	new = malloc(len + bin_len + 2);
	if (!new)
		return (NULL);
	ft_strlcpy(new, path, len + 1);
	ft_strcat(new, "/");
	ft_strcat(new, bin);
	return (new);
}

// free all & exit
int	abort_fill(char **path_list)
{
	int	i;

	i = 0;
	while (path_list[i])
		free(path_list[i++]);
	return (-1);
}

int	fill_path_list(char **path_list, char *path, char *bin, int bin_len)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (path[i])
	{
		if (path[i] != ':')
		{
			if (path_list)
			{
				path_list[count] = create_path(path + i, bin, bin_len);
				if (!path_list[count])
					return (abort_fill(path_list));
			}
			count++;
			while (path[i] && path[i] != ':')
				i++;
		}
		else
			i++;
	}
	if (path_list)
		path_list[count] = NULL;
	return (count);
}

char	**split_path(char *path, char *bin)
{
	char	**path_list;
	int		bin_len;

	path_list = malloc(sizeof(char *) * (fill_path_list(NULL, path, NULL, 0) + 1));
	if (!path_list)
		return (NULL);
	bin_len = ft_strlen(bin);
	if (fill_path_list(path_list, path, bin, bin_len) == -1)
		return (NULL);
	return (path_list);
}

char	**get_path_list(char **ep, char *bin)
{
	int		i;

	i = 0;
	while (ep[i])
	{
		if (!ft_strncmp(ep[i], "PATH=", 5))
			return (split_path(ep[i] + 5, bin));
		i++;
	}
	return (NULL);
}

// Test path list for access, returns first valid path (or NULL)
char	*test_paths(char **path_list, int mode)
{
	int		i;

	i = 0;
	while (path_list[i])
	{
		if (!access(path_list[i], mode))
			return (path_list[i]);
		i++;
	}
	return (NULL);
}*/

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

static char	*test_path(char *path, char *bin)
{
	char	*pathname;
	size_t	bin_len;

	bin_len = ft_strlen(bin);
	pathname = build_pathname(path, bin, bin_len);
	if (!pathname)
		return (NULL); //TODO might exit here instead (if a malloc fails, makes more sense to stop everything)
	if (!access(pathname, X_OK))
		return (pathname);
	free(pathname);
	return (NULL);
}

// Returns valid pathname for bin or NULL
char	*get_bin_path(char *env_path, char *bin)
{
	int		i;
	char	*bin_path;

	i = 0;
	while (env_path[i])
	{
		if (env_path[i] != ':')
		{
			bin_path = test_path(env_path + i, bin);
			if (bin_path)
				return (bin_path);
			while (env_path[i] && env_path[i] != ':')
				i++;
		}
		else
			i++;
	}
	return (bin_path);
}

// Finds and return path var in env
char	*get_env_path(char **ep)
{
	int	i;

	i = 0;
	while (ep[i])
	{
		if (!ft_strncmp(ep[i], "PATH=", 5))
			return (ep[i] + 5);
		i++;
	}
	return (NULL);
}

// This is where it gets complicated with awk { }
// We'll do it naively first (one line)
// We'll expand on this when I understand the subtleties
char	**get_cmd_argv(char *cmd)
{
	return (ft_split(cmd, ' '));
}

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
}

int	main(int ac, char **av, char **ep)
{
	char	*env_path;
	char	*bin_path;
	char	**bin_argv;

	if (ac != 2)
		return (0);
	env_path = get_env_path(ep);
	if (!env_path)
		return (1);
	bin_argv = get_cmd_argv(av[1]);
	if (!bin_argv)
		return (2);
	bin_path = get_bin_path(env_path, bin_argv[0]);
	if (!bin_path)
	{
		free_strs(bin_argv);
		return (3);
	}
	execve(bin_path, bin_argv, ep);
	// We're not freeing bin_path or bin_argv
	// We can't do it without forking
}
