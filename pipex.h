/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 09:49:50 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/10 18:59:49 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define SH_NAME	"zsh"
# define HEREDOC	"here_doc"
# define DEVNULL	"/dev/null"

# define PIPE_MAX	65536

# define ERRMALLOC	"malloc failure"
# define ERRPIPE	"pipe failure"
# define ERRDUP		"dup failure"
# define ERRFORK	"fork failure"

# define ERRPATH	"command not found"

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>

# include "libft.h"

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
 * 1. Init:
 *		- Verify args
 *		- Figure out path from envp
 *		- Figure out whether we're doing heredoc
 *		- Figure out the number of commands to execve
 *	  We are not piping, duping or building any command argv/pathname yet
 * 2. Main program loop:
 *		- For each command:
 *			- Open file/heredoc or re-use last pipe's read to dup stdin
 *			- Open file or create a new pipe, dup its write to stdout
 *			- Build its argv to send to execve
 *			- Build its pathname
 *			If any of the 4 previous steps fail, print an error, go to next cmd
 *			- Fork, child closes other end of stdout pipe, and execs cmd
 * 3. Exit:
 *		- All fds were progressively closed
 *		- Wait for all children and free their argv + pathname
 *		- Done \(^-^)/
 *
 * Note that most operations are done in the parent process, that is before
 *	any forks. This behavior diverges from bash's (it seems that bash dups
 *	in its forks right before execve).
 * The reason is to simplify program structure. The only fd that the child has
 *	to close this way is the other end of its stdout pipe (next cmd's stdin).
 *
 * Also note that soft errors (can't open file, pipe or dup failure, no path
 *	found) do not cause program failure, they only stop the current cmd's
 *	execution. The only critical error for which we stop completely is malloc
 *	failure.
 *
 * Finally, note that we do not wait for children until the end of the loop.
 *	As such, all cmds are executed in parallel. A "sleep 1 | sleep 1" won't
 *	cause a total sleep of 2. A "yes | head" command won't run forever.
*/

/*	t_cmd struct
 *
 * Contains cmd info for waiting and freeing at program exit. Self-explanatory.
 * Used as an array of [numcmd] length.
*/
typedef struct s_cmd
{
	int		pid;
	char	*pathname;
	char	**argv;
}	t_cmd;

/*	t_pipex struct
 *
 * Main program struct. Contains data relevant throughout all of the program.
 *
 * ac			-
 * av			-
 * ep			-
 * path			Starts at index 5 (skips 'P' 'A' 'T' 'H' '=')
 *				Can be NULL, commands will just not be found
 * stdin_next	Holds read end of the most recently opened pipe
 *				It is closed in the child and duped to next command's stdin
 *				All other fds are duped right after opening, then closed
 * dev_null		Contains /dev/null fd (only if we need it)
 * numcmd		-
 * heredoc		1 if heredoc is on, 0 if not
 *				Its value is used directly in index calculation to fetch strings
 * arrcmd		-
*/
typedef struct s_pipex
{
	int		ac;
	char	**av;
	char	**ep;
	char	*path;
	int		stdin_next;
	int		dev_null;
	int		numcmd;
	int		heredoc;
	t_cmd	*arrcmd;
}	t_pipex;

// pipex.c
void	pipex(t_pipex d);

// init.c exit.c
void	init_pipex(int ac, char **av, char **ep, t_pipex *d);
void	exit_pipex(t_pipex d, int exitval);

// usage.c
int		usage(void);
int		usage_heredoc(void);

// error.c
void	ft_perror(char *s);
void	ft_perror_syscall(char *s);
void	ft_perror_path(char *s);
void	critical_error(t_pipex d, char *s);

// open.c
//	Used in resolve_stdin() resolve_stdout()
int		open_devnull(t_pipex *d, int *fd);
int		open_infile(t_pipex d, int *fd); //TODO HEREDOC
int		open_outfile(t_pipex d, int *fd);

// pipe.c argv.c path.c
int		resolve_stdin(t_pipex *d, int i);
int		resolve_stdout(t_pipex *d, int i);
int		resolve_argv(t_pipex d, int i);
int		resolve_pathname(t_pipex d, int i);

// util.c
//	Simplify syscall usage (e.g set fd to -1 after close)
void	free_argv(char **argv);
void	safe_close(int *fd);
int		safe_pipe(int pipefd[2]);
int		safe_dup2(int *old, int new);

#endif
