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

```
After cloning the directory to your local machine:
---

1. cd into rshell directory

2. call make

3. cd into /bin

4. call rshell

Bugs
===
1. ls cannot handle one or multiple file arguments 

2. Command "cd" fails, returns "execvp: no such file or directory"
