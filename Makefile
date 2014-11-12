all:bin rshell cp ls

bin:
	mkdir bin



rshell:
	g++ -Wall -Werror -ansi -pedantic src/rshell.cpp -o ./bin/rshell

cp:
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o ./bin/cp

ls:
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o ./bin/ls
