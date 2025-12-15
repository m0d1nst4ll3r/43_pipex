# 43\_pipex

Project complete

Turn-in wasn't perfect: a problem with still reachables when reading stdin, a problem with "." ".." commands and execve failing, and a lot of stuff not handled (but out of scope), like quotes, backslashes, etc...

Everything except out-of-scope stuff should be fixed in this final version. V2 will probably never see the day (quotes etc..., just too time-consuming and not my passion).

### Improvements

Annoying parsing stuff that tbh would be its own bag of fun except we'll do it anyway for minishell, I guess:
- Handle quotes and double quotes
	- Sending echo "test" should print test, not "test"
	- Sending cat "fi le" should try to read from file fi le, not "fi then le"
	- Same for ' as in echo 'test' cat 'fi le'
- Handle backslashes
	- Sending echo \t\e\s\t should print test, not \t\e\s\t
- Handle way more stuff like this... Ran out of ideas pretty fast

Doing it the other way, as in create all pipes etc... *before* forking
- Really not sure if this can be considered an improvement?
