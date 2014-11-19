#include <iostream>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
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

void redirect(char ** cmd, int numpipes)
{
	int l_than = -1;
	int g_than = -1;
	int gg_than = -1;
	for(int i = 0;cmd[i] != NULL;++i)
	{

		char * input_file;
		char * output_file;

		char *args[1024];
		int argc = 0;
		char *tok = strtok(cmd[i], " \t\n\r");
		while(tok != NULL)
		{
			if(strcmp(tok, "<") == 0)
			{
				++l_than;
				if(l_than >0)
				{
					cout << "Error: Invalid amount of input operators" << endl;
					exit(1);
				}
				char * input_file = strtok(NULL, " \t");
				++argc;
				break;
			}
			else if(strcmp(tok,">>") == 0)
			{
				++g_than;
				if((g_than + gg_than) > 0)
				{
					cout << " Error: Invalid amount of output operators" << endl;
					exit(1);
				}
				char * output_file = strtok(NULL, " \t");
				++argc;
				break;
			}
			else if(strcmp(tok, ">") == 0)
			{
				++gg_than;
				if((g_than + gg_than) > 0)
				{
					cout << " Error: Invalid amount of output operators" << endl;
					exit(1);
				}
				char * output_file = strtok(NULL, " \t");
				++argc;
				break;
			}
			else
			{
				args[argc] = tok;
			}
			argc++;
			tok = strtok(NULL, " \t");
		}
		args[argc] = NULL;
			
		int fd[2];
		if(!(cmd[i+1] == NULL))
		{	
			pipe(fd);
		}
		
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
		}
		else if(pid == 0) //child
		{
			if(i != 0)
			{
				if(-1 == dup2(fd[0],0))
					perror("dup2");
			}
			if(!(cmd[i+1]==NULL))
			{
				if(-1 == dup2(fd[1],1))
					perror("dup2");
			}
			if(i = 0 && l_than == 0)
			{
				int in = open(input_file, O_RDONLY);
				if(in == -1)
					perror("open");
				if(-1 == close(0))
					perror("close");
				if(-1 == dup(in))
					perror("dup");
			}

			if(cmd[i+1] == NULL && g_than == 0)
			{
				int out = open(output_file,O_WRONLY | O_CREAT);
				if(-1 == out)
					perror("open");
				if(-1 == close(1))
					perror("close");
				if(-1 == dup(out))
					perror("dup");

			}
			if(cmd[i+1] == NULL && gg_than == 0)
			{
				int out = open(output_file,O_WRONLY | O_CREAT|O_APPEND);
				if(-1 == out)
					perror("open");
				if(-1 == close(1))
					perror("close");
				if(-1 == dup(out))
					perror("dup");

			}


			if(-1 == execvp(args[0], args))
				perror("execvp");
			exit(1);	
		}
		else//parent
		{}



	}
	for(int i =0; cmd[i] != NULL;++i)
	{
		if(-1 == wait(0))
		{
			perror("wait");
		}
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

		if(strcmp(command_c, "exit") == 0)
		{
			exit(1);
		}	

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

		int amnt_pipes = x - 1;


		redirect(cmds,amnt_pipes);

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
