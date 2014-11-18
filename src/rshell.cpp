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

bool exec_vp(string commands)
{
	const int y = commands.size() + 1;
	char* stuff = new char[y];//Stores command line as c strings
	strcpy(stuff,commands.c_str());

	char** cmd= new char*[y];//Stores the parsed command line
	int x = 0;
	char* tok = strtok(stuff, " \n\t\r");//Parses the c_strings

	while(tok != NULL)//Adding tokens into storage
	{
		if(strcmp(tok, "exit") == 0)//if an argument is exit
					    //it will exit
		{
			exit(0);
		}
		cmd[x] = tok;
		tok = strtok(NULL," \n\t\r");
		//cout << cmd[x] << " ";
		++x;
	}
	
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
	return false;
}

void redirect(char ** cmd)
{
	for(int i = 0;cmd[i] != NULL;++i)
	{
		bool in = false;
		bool out = false;
		bool outout = false;



		



	}
}


int main()
{
	while(true)
	{
		if(NULL == getlogin())//Display login user
		{
			perror("getlogin");
		}
		else
		{
			cout << getlogin();
		}
		char host_name[64] ={0};
		if(-1 == gethostname(host_name,64))//Display host name
		{
			perror("hostname");
		}
		else
		{
			cout << "@" << host_name;
		}
		string commands;//Receive command line
		cout << "$";
		getline(cin,commands);

		//Removing comment
		int size_str = commands.find("#");
		commands = commands.substr(0,size_str);
		
		

		//Seperating by pipes
		

		const int max_size = commands.size();
		char* command_c = new char[max_size];
		strcpy(command_c, commands.c_str());


		char ** cmds = new char *[max_size];
		int x = 0;
		char* tok = strtok(command_c, "|\n\t\r");
		while(tok!=NULL)
		{
			cmds[x] = tok;
			tok = strtok(NULL, "|\n\t\r");
			++x;
		}
		cmds[x] = NULL;
		/*for(int y = 0;y<x;++y)
		{
			cout << cmds[y] << endl;
		}*/
		
		


/*
		//Seperating by connectors
		int connect = 0;
		bool no_connect = true;
		bool next_it = true;
		string flag = "";
		while(no_connect)
		{
			if(next_it)
			{
				if(commands.find("||")!=string::npos)
				{
					connect = commands.find("||");
					flag = "||";
				}
				else if(commands.find("&&")!=string::npos)
				{
					connect = commands.find("&&");
					flag = "&&";
				}
				else if(commands.find(";")!=string::npos)
				{
					connect = commands.find(";");
					flag = ";";
				}
				else
				no_connect = false;
				if(flag == "||")
				{
					string curr = commands.substr(0,connect);
					if(exec_vp(curr)==true)
					next_it = false;
					commands = commands.substr(connect+2);
				}
				else if(flag == "&&")
				{
					string curr = commands.substr(0,connect);
					if(exec_vp(curr)==false)
					next_it = false;
					commands = commands.substr(connect+2);
				}
				else if(flag == ";")
				{
					string next = commands.substr(0,connect);
					if(exec_vp(next));
					commands = commands.substr(connect+1);
				}
				else
					exec_vp(commands);
				flag = "";
			}
			else
			next_it = true;
		}
	*/
	}


	return 0;
}
