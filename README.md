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

### Thoughts/Progress

Assuming we're doing the infinite cmd bonus (very easy) but not heredoc yet:

First have to decompose args. 1st should be file, last should be file, in-between should be cmds.

There are always argc - 3 commands.

For each pair of commands, need a pipe. Need num_commands - 1 pipes.

For each command, need to decompose it in its own argv. Note that it might be pretty hard to handle awk { } (we'll handle this later).

For each command, need to find its pathname.

For the two files, need to open them (O_RDONLY, O_WRONLY, O_CREAT, with the right mode 644 with bit mask).

Need to fork as many times as we have commands. Each fork needs to dup its input and output.

- 1st command dups file to input, output into 1st pipe's write.
- 2nd command dups 1st pipe's read to input, output into 2nd pipe's write.
- 3rd command dups 2nd pipe's read to input, output into 3nd pipe's write.
- etc...
- nth command dups (n-1)th pipe's read to input, output into file.

Confused about which fds need to be closed and why (gotta research this more). For 2 commands, 1st needs to close the pipe's read and the 2nd needs to close the pipe's write, but I don't clearly understand why yet.

### Error management

What happens if:

- file1 doesn't exist/can't be read

- file2 doesn't exist/can't be written to

- cmd1 or cmd 2 aren't found anywhere

- PATH is empty

- PATH is not even set (unset PATH)

- Could any functions I'm using fail? e.g fork, execve, dup2, pipe
  - pipe can fail (-1 ret)
  - fork can fail (-1 ret)

### Final program steps

Depends on error management, have to research errors more.
