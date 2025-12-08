/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 14:30:39 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 12:37:27 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

<<<<<<< HEAD
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

char	**get_cmd_argv(char *cmd)
{
	return (ft_split(cmd, ' '));
}

// Ex print:
//	zsh: no such file or directory: nofile
void	ft_perror(char *s1, char *s2)
{
	ft_fprintf(2, "%s: %s: %s\n", s1, strerror(errno), s2);
}

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

// Child only needs to close read end of next pipe
void	exec_child(t_pipex d, int i, int to_close)
{
	close(to_close);
	execve
}

// Returns fd pointing to infile
// fd might be a pipe read end (heredoc)
int	open_infile(t_pipex d)
{
	// how do we handle heredoc
	// we need to open stdin and read line by line
	// get next line will be perfect here
	// not sure whether we should stop reading at read returning 0
	// the shell doesn't really seem to do that (or sometimes it does sometimes it doesn't)
	// actually, zsh never allows you to press ctrl D to end input
	// bash does allow you but only if the line is empty, then it prints a warning that
	//	it encountered EOF when it was expecting the limiter
	// sh allows you to ctrl D on empty lines AND non-empty (but have to press twice)
	//	and it doesn't give any warning or message or anything
	// so any behavior like any of these shells or any in-between should be ok
	// the simplest is probably to handle like zsh, keep reading forever until limiter
	// we have to store these lines returned by gnl somewhere
	// we might, btw, have to add a gnl function to free a fd in case after reading from stdin
	//	there is memory left unfreed
	// the way I did my gnl, it should be fine, but if not, I'll have to literally modify
	//	my gnl function to accept 1 more arg and call it like I do open (e.g gnl(fd, GNL_OPEN)
	//	or gnl(fd, GNL_FREE)
	// so anyway call gnl in a while and look for the line it returns, if it contains only LIMITER
	//	or it contains LIMITER with JUST a \n behind, we stop
	// we have to save these lines somewhere. We can either save them as is, or concatenate them
	//	into a big string. In this case, it might be more interesting to store them some other way
	//	instead of using gnl (directly store them in a buffer list for example)
	// once we reach LIMITER we stop, look at how many bytes we have, if it is > max pipe (65536)
	//	we have to write to a temporary file (we can just name it .here_doc or whatever) after having
	//	opened it in WRONLY, write everything to it, close the fd and reopen the file in RDONLY
	//	and use that fd. it will be dup2 in the child process
}

// Will import libmalloc to free all
// This is basically only useful for malloc failure, as any other type of
//	failure only causes a "soft error": the cmd will not execute but we
//	still continue execution.
void	error_critical(char *err)
{
	ft_fprintf(2, "Critical error: %s: %s\n", err, strerror(errno));
	exit(1);
}

void	init_data(int ac, char **av, char **ep, t_pipex *d)
{
	d->ac = ac;
	d->av = av;
	d->ep = ep;
	d->heredoc = 0;
	if (!ft_strcmp(av[1], HEREDOC))
		d->heredoc = 1;
	d->numcmd = ac - 2;
	if (d->heredoc)
		d->numcmd = ac - 3;
	d->arrcmd = malloc(sizeof(*(d.arrcmd)) * numcmd);
	if (!d->arrcmd)
		error_critical(ERRMALLOC);
	init_arrcmd(d->arrcmd, d->numcmd);
}

void	close_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

// d short for data (since we will be writing it a lot)
=======
/*	|-----------------<(^-^)/-------/ Pipex V1 \-------\(^-^)>-----------------|
 *
 *	|------/    Program    \------|
 *	|------\  description  /------|
 *
 * Program takes 4 or more arguments.
 *
 * Shortest arguments:
 *	./pipex file1 cmd1 cmd2 file2
 *
 * Long arguments:
 *	./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
 *
 * Heredoc arguments:
 *	./pipex here_doc LIMITER cmd1 cmd2 file2
 *
 * Program will then execve every command and redirect fds so that:
 *	- file1 goes into cmd1 stdin
 *	- cmd1 stdout goes into cmd2 stdin
 *	- cmd2 stdout goes into cmd3 stdin
 *	- etc...
 *	- cmd(n-1) stdout goes into cmdn stdin
 *	- cmdn stdout goes into file2
 *
 * file1 is opened with O_RDONLY. If file1 cannot be opened, cmd1 will not
 *	be executed (but other commands will, although cmd2 stdin will be empty).
 * file2 is opened with O_WRONLY | O_CREAT | O_TRUNC - if it exists already,
 *	it is truncated and written over. If it cannot be opened, cmdn will not
 *	be executed (but other commands will).
 *
 * Commands are split around spaces (for V1, V2 will have double quote & awk
 *	management). The first word in the command is taken as the binary. We
 *	look into PATH and use the first valid binary with execute permissions.
 *
 * Commands are executed with execve, we send them the argv we just split
 *	(beginning with their name). They also inherit the environment. Should
 *	a command not be found, others will still execute (like incorrect files).
 *
 * A "here_doc" file will cause pipex to open stdin instead for heredoc input.
 *	It will wait for LIMITER to close stdin. If input is too lengthy for a
 *	pipe buffer size (taken to be 65536), a temporary file will be created.
 * Additionally, "here_doc" also causes pipex to ADD to file2 instead of
 *	truncating it. As such file2 is opened with O_WRONLY | O_CREAT | O_APPEND.
 *
 * Note that created files have their chmod set to 0644.
 *
 * Note: some commands built into the shell might work natively without a PATH,
 *	such as `pwd`, `cd`, `echo`, but pipex will not find them without a PATH.
 *	It might not even find these commands at all (such as `cd`) if they do not
 *	have an executable bin somewhere in PATH.
 *
 *	|------/    Program    \------|
 *	|------\     steps     /------|
 *
 * 0. Note that mallocs are all made in the parent so they can be freed after
 *	   `wait`ing for all the children. Also note that `open`s and `pipe`s are
 *	   all closed as fast as possible since they will be duplicated in children
 *	   (and therefore also need to be closed in the children if not in the
 *	   parent).
 *	  There are many things to do before `execve`ing the processes. They can
 *	   be done in almost any order. The order described is the one I chose, but
 *	   there are many more possibilities.
 *	  Description of what needs to be done:
 *		- Split commands so we can send argv to `execve`
 *		- Search for a valid bin in PATH to be sent to `execve`
 *		- Create pipes between commands (n - 1 pipes for n commands)
 *		- Attempt to open files for cmd1 and cmdn
 *	  In the case of here_doc, the very first operation has to be reading stdin.
 *	  Nothing will happen (no command will run) until heredoc has been resolved.
 * ...
 * ...
 * ...
 *
 * Get numcmd from args (whether heredoc or not, if not, argc - 3, if so, argc - 4)
 * Handle heredoc if needed
 *	--> stuff..
 * Start a while which will run numcmd times
 * For each iteration, we want to execve the cmd in a fork, so we have to fork
 * Before forking we have to build argv and find the bin's path
 * bin path and argv are both malloc'd, we need to remember those mallocs.
 * they should be memorized in a structure and we will do an array of that struct (of numcmd len)
 * that struct should have argv, pathname, stdin, stdout
 * we obvly need to open files (for 1st and last) and pipes - redirection is done in children
 * for non-1st and non-last we only need to:
 *	1. Open a pipe (so cmd stdout can be written to new pipe write) - we cant close anything
 *		on that pipe yet
 *	2. Close the write of the last pipe made (only need read for this cmd)
 *	3. Close the read of the before-last pipe made
 * At i == 0 and i == numcmd - 1 (at first and last cmds) we do special things (files)
 * 
*/
>>>>>>> 7f44a8dd90e20f603eb6f5e96c1bbd417e5deff2
int	main(int ac, char **av, char **ep)
{
	int		i;
	t_pipex	d;

	if (ac < 5)
		return (0);
	i = 0;
	init_data(ac, av, ep, &d);
	while (i < d.numcmd)
	{
		// IN ORDER
		// 1. STDIN
		//	a. i == 0 ? open infile and put fd in stdin
		//		- open error ? print error, don't fork
		//		- stdin takes -1 for next cmd
		//	b. i != 0 ? stdin was written in last iteration so do nothing
		//	c. dup stdin to STDIN, close stdin
		// 2. STDOUT
		//	a. i == n - 1 ? open outfile
		//		- open error ? print error, don't fork
		//		- this is last cmd anyway, close stdin from last iteration
		//	b. i != n - 1 ? open pipe
		//		- pipe error ? print error, don't fork
		//		- stdin takes -1 for next cmd
		//	c. dup pipe[1] to stdout, close pipe[1]
		// 3. ARGV & PATHNAME
		//	a. split argv
		//		- malloc error ? critical exit, close stdin (double close is protected)
		//	b. get pathname from argv[0]
		//		- malloc error ? same as above
		//		- critical exit should free all mallocs anyway (once I plug in libmalloc)
		// 4. FORK
		//	a. memorize the pid in array
		//	b. fork closes the only fd that couldn't be closed before call: stdin (for next iter)
		//	c. fork does an execve with pathname, argv, envp, its stdin and stdout are set
		if (!i)
		{
			arrcmd[i].stdin_fd = open_infile(d);
			if (arrcmd[i].stdin_fd == -1)
			{
				ft_perror(SH_NAME, );
			}
		}
		else if (i + 1 == numcmd)
		arrcmd[i].pathname = 
		arrcmd[i].pid = fork();

		i++;
	}
}
