#include <iostream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

using namespace std;

int main()
{
	while(true)
	{
		string commands;//Receive command line
		cout << "$";
		getline(cin,commands);
		const int y = commands.size() + 1;
		char* stuff = new char[y];//Stores command line as c strings
		strcpy(stuff,commands.c_str());
		char** cmd= new char*[y];//Stores the parsed command line
		int x = 0;
		char* tok = strtok(stuff, " ;\n\t\r");//Parses the c_strings
		while(tok != NULL)//Adding tokens into storage
		{
			if(strcmp(tok, "exit") == 0)//if an argument is exit
						    //it will exit
			{
				exit(0);
			}
			cmd[x] = tok;
			tok = strtok(NULL," ;\n\t\r");
			//cout << cmd[x] << " ";
			++x;
		}
		
		//cout << x << " " << x+1 << endl;Checks how many arguments and
						//which would be the null 
						//argument
		cmd[x] = NULL;//Makes last argument the null character
		int pid = fork();
		if(pid == 0)
		{
			//cout << "This is the child" << endl;
			if(-1 == execvp(cmd[0], cmd))
			perror("execvp");
			exit(1);
		}
		else if (pid == -1)//If fork fails
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
