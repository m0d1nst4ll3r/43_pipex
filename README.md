# 43\_pipex

- Known functions:	open, close, read, write, malloc, free, strerror, exit, wait
- Google this:		perror, access, dup, dup2, execve, fork, pipe, unlink, waitpid

- perror: prints strerror(errno), can provide a string which it will put behind a colon
- access: check file perms, returns 0 on success, -1 on error, sets errno on error, takes a file (pathname) and a mode (mask of R\_OK W\_OK X\_OK -or- F\_OK), F\_OK checks if the file exists
- dup: duplicate a file descriptor (returns a new fd, both old and new link to the same file)
- dup2: same as dup but you specify the new fd to be used (if it was in use, it's closed before being reused)
- execve: execute a new program with pathname (which replaces current program), can pass an array of strings to be sent as argv to the new program, can also send a new environment (array of str in the form key=value)
- fork: duplicates program then returns the pid of the child to the parent and 0 to the child
- pipe: takes an array of 2 ints and writes in it the fds of a new pipe, [0] has the read end, [1] the write end - data written in the write end is buffered by the kernel until it is read from the read end. returns 0 on success, -1 on error
- unlink: deletes a name from the filesystem, if it was the last link to a file and the file isn't open in any process, the file is deleted
- waitpid: like wait but in minitalk you use wait to pause in wait of a signal that will interrupt the waiting, here you would use wait to pause in wait for a child process to terminate, waitpid gives you the option of waiting for a specific child process

### Goal

./pipex file1 cmd1 cmd2 file2

Should perform exactly the same as

< file1 cmd1 | cmd2 > file2

file1 > cmd1 > cmd2 > file2

### Bonuses

./pipex file1 cmd1 cmd2 cmd3 cmd4 ... cmdn file2

use heredoc (?)

### Thoughts

If they give us access, it probably means we need to use it to check for the files we're using as input and output (if we have read allowed for input and write for output). But open would already return -1 with errno set so what's the use?

We'll have to fork our program into 2 copies and each copy will have to execve a program. But then those programs need to communicate so we need to wire the output (stdout) of the first program to the input (stdin) of the other using a pipe. Probably.

For the files, to wire them to the programs we need to open them first (one in rdonly, the other in wronly), then we need to dup2 the fd returned by open into the child process' stdin fd (STDIN\_FILENO) or stdout (STDOUT\_FILENO). Probably.

Not sure what unlink is used for.

waitpid would be used to wait for the child processes to terminate. Not sure when or how we need to wait, also not sure how the pipe would be used exactly. Apparently pipes have a max size to their buffer, so they should be read instantly as the other program is writing into them. But idk how to tell for example a `wc -l` program to wait for `ls -l` to have finished writing before executing. In fact, I can't really tell wc or ls to do anything in particular, right? Even how to wire their stdin/stdout is not clear to me.

Will have to parse args very carefully, bonusless means 5 args exactly, bonus means at least 5 but possibly more. For execve, need to split the string by spaces to pass args to the new program (conveniently, the first arg should be the program name, which makes sense).

In case of non-existing file or no perms or it's a directory etc etc, need to handle errors exactly like the shell when it does `< file1 cmd1 | cm2 > file2`.

### Exploring

Found a solution to the env thing. You can have 3 arguments to a main function. argc, argv, and *envp*. Which makes sense given the arguments of the execve function (pathname, then argv and envp). envp is exactly like argv, it ends with NULL, and every string is a `key=value` format. You can simply parse this to find the `PATH=` string then decompose the paths and test them one by one.

Paths should be tested from left to right like the shell.

### Error management

What happens if:

- file1 doesn't exist/can't be read

- file2 doesn't exist/can't be written to

- cmd1 or cmd 2 aren't found anywhere

- PATH is empty

- PATH is not even set (unset PATH)

- Could any functions I'm using fail? e.g fork, execve, dup2, pipe
