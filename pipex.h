/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 09:49:50 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/08 11:55:35 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#define SH_NAME		"zsh"
#define HEREDOC		"here_doc"

#define ERRMALLOC	"Malloc failure"

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
 *	pipe buffer size (taken to be 66536), a temporary file will be created.
 * Additionally, "here_doc" also causes pipex to ADD to file2 instead of
 *	truncating it. As such file2 is opened with O_WRONLY | O_CREAT | O_APPEND.
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
 * Note: the only thing we're doing in the child is dup2 of stdin/stdout
 * stdin/stdout are automatically closed at exit so it should be fine to leave them open
 * all mallocs are freed in parent
 * child inherits very few fd's from parent, but they should still be closed, especially
 *	after dup, so it would be a good idea to have a free_all_fd function
 *	also since whatever the parent closed has the value -1 we can just close a couple fds per child
*/

// we can dup2 in parent instead of child. this way a dup error can be handled a bit better.
// we should do as much as possible in the parent instead of the child
// the only thing we can't do in the parent is close the read of the
//	out-pipe, since we need it for the next cmd
// this means having a stdin_fd and stdout_fd in the struct might be useless

// PID is filled at fork
// pathname contains bin path (NULL if no bin found)
// argv contains split command
// stdin_fd and stdout_fd contain the original fd before dup2
typedef struct s_cmd
{
	int		pid;
	char	*pathname;
	char	*argv;
}	t_cmd;

// numcmd is the number of commands to be run
// heredoc is non-0 if it was defined in args
// arrcmd is the array of cmds (containing relevant cmd data such as pathname)
typedef struct s_pipex
{
	int		ac;
	char	**av;
	char	**ep;
	int		numcmd;
	int		heredoc;
	t_cmd	*arrcmd;
}	t_pipex;

#endif
