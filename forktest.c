#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

// read - write
int	main(int ac, char **av, char **ep)
{
	char	**bin_av;
	int		fds[2];
	int		ret;
	int	filefd;

	(void)ac;
	(void)av;
	pipe(fds);
	ret = fork();
	if (ret)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO); // ls out goes into wc
		filefd = open("infile", O_RDONLY);
		dup2(filefd, STDIN_FILENO);
		bin_av = malloc(16);
		bin_av[0] = "ls";
		bin_av[1] = NULL;
		execve("/usr/bin/cat", bin_av, ep);
	}
	else
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO); // wc in is wired to ls out
		filefd = open("outfile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(filefd, STDOUT_FILENO);
		bin_av = malloc(16);
		bin_av[0] = "wc";
		bin_av[1] = NULL;
		execve("/usr/bin/wc", bin_av, ep);
	}
}
