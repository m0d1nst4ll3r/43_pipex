#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

// read - write
int	main(int ac, char **av, char **ep)
{
	int		fds[2];
	int		ret;
	int		ret2;
	int	filefd;

	(void)ac;
	(void)av;
	pipe(fds); // creating pipe
	filefd = open("infile", O_RDONLY);
	dup2(filefd, STDIN_FILENO); // duping in parent
	close(filefd);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);
	ret = fork(); // first fork, doing ls
	if (!ret)
	{
		close(fds[0]); // no choice but to close read end in child
		execve("/usr/bin/ls", av, ep); // sending parent av, doesn't matter
	}
	else
	{
		//still in parent
		filefd = open("outfile", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(filefd, STDOUT_FILENO);
		close(filefd);
		dup2(fds[0], STDIN_FILENO); // still duping in parent
		close(fds[0]);
		ret2 = fork();
		if (!ret2)
		{
			// nothing to close
			execve("/usr/bin/wc", av, ep);
		}
	}
}
