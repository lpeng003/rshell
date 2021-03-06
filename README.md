RSHELL
===
Project source can be downloaded from https://github.com/lpeng003/rshell.git
===

File List
===
```
Makefile

LICENSE

README.md

/src

/tests
```
```
/src:

rshell.cpp
cp.cpp
ls.cpp
```
```
/tests:
exec.script
ls.script
pipe.script
signals.script

```
After cloning the directory to your local machine:
---

1. cd into rshell directory

2. call make

3. cd into /bin

4. run ./rshell

Bugs
===
1. ls cannot handle one or multiple file arguments 
	ls will simply ignore that file paramenter and run in the current directory

2. Command "cd" will ignore any additional parameter after the first directory. It will also return if no directory is specified.

3. ls may have incorrect spacing when running certain commands

4. rshell can not handle 3 or more commands with pipes
	process will hang, possibly due to failure to properly pipe to next commands

5. rshell at 2 arguments will possibly work, but will still have a hanging process.

6. Pipes currently removed from rshell.

7. ^C is ignored by rshell, will simply print ^C^C repeatedly.
