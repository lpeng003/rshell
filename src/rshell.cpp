#include <iostream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main()
{
	while(true)
	{
		string commands;
		cout << "$";
		getline(cin,commands);
		int y = commands.size() + 1;
		char* stuff = new char[y];
		strcpy(stuff,commands.c_str());
		char * cmd[y];
		int x = 0;
		char* tok = strtok(stuff, " ;\n\t\r");
		while(tok != NULL)
		{
			if(strcmp(tok, "exit") == 0)
			{
				exit(0);
			}
			cmd[x] = tok;
			tok = strtok(NULL," ;\n\t\r");
		//	cout << cmd[x] << " ";
			++x;
		}
		//cout << x << " " << x+1 << endl;
		cmd[x+1] = NULL;
		int pid = fork();
		if(pid == 0)
		{
			//cout << "This is the child" << endl;
			if(-1 == execvp(cmd[0], cmd))
			perror("execvp");
			exit(1);
		}
		else if (pid == -1)
		{
			perror("fork");
		}
		else
		{
			if(-1 == wait(0))
			{
				perror("wait");
			}
			//cout << "This is the parent" << endl;
		}

	}


	return 0;
}
