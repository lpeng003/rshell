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
#include <signal.h>

using namespace std;

void cd(const char *path)
{
	if(path == NULL)
	{
		cout << "Error:No path specified" << endl;
		return;
	}
	if(-1 == chdir(path))
	{
		perror("chdir");
	}	

}

bool exec_v(string commands)
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
		if(strcmp(tok,"cd") ==0)
		{
			tok = strtok(NULL," \n\t\r");
			cd(tok);
			delete[] stuff;
			return false;
		}
		cmd[x] = tok;
		tok = strtok(NULL," \n\t\r");
		//cout << cmd[x] << " ";
		++x;
	}
	cmd[x] = NULL;//Makes last argument the null character

	char* env = getenv("PATH");
	string path = env;
	const int z = path.size() + 1;
	char* newenv = new char[z];
	strcpy(newenv,path.c_str());
	vector <char *> paths;
	tok = strtok(newenv,":\n\t\r");

	char dir[1024] = "/.";
	paths.push_back(dir);
	while(tok != NULL)
	{
		paths.push_back(tok);
		tok = strtok(NULL,":\n\t\r");


	}

	int pid = fork();
	if(pid == 0)
	{
		//cout << "This is the child" << endl;
		
		for(unsigned i = 0; i<paths.size(); ++i)
		{
			string first = paths.at(i);
			string end = cmd[0];
			string backslash = "/";
			int big =strlen(paths.at(i))-1;
			if(paths.at(i)[big] != '/')
			{
				first.append(backslash);
				first.append(end);
				const char *new_path = first.c_str();
				if(-1 != execv(new_path,cmd))
					exit(1);
			}
			else
			{
				first.append(end);
				const char *new_path1 = first.c_str();
				if(-1 != execv(new_path1, cmd))
					exit(1);
			}
		}
		perror("execv");
		//if(-1 == execvp(cmd[0], cmd))
		//perror("execvp");
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
	delete[] newenv;
	delete[] stuff;
	return false;
}

void redirect(char ** cmd, int size)
{
	/*int l_than = -1;
	int g_than = -1;
	int gg_than = -1;

	int fdbefore[2];
	int fdafter[2];

	for(int i = 0;i<size;++i)
	{
		//cout << "Size is: " << size <<endl;
		//cout << "i is: " << i << endl;

		char * input_file;
		char * output_file;

		char *args[1024];
		int argc = 0;
		char *tok = strtok(cmd[i], " \t\n\r");
		while(tok != NULL)
		{
			//cout << tok << endl;
			if(strcmp(tok, "<") == 0)
			{
				++l_than;
				if(l_than >0)
				{
					cout << "Error: Invalid amount of input operators" << endl;
					exit(1);
				}
				input_file = strtok(NULL, " \t");
				++argc;
				break;
			}
			else if(strcmp(tok,">>") == 0)
			{
				++gg_than;
				if((g_than + gg_than) > 0)
				{
					cout << " Error: Invalid amount of output operators" << endl;
					exit(1);
				}
				output_file = strtok(NULL, " \t");
				++argc;
				break;
			}
			else if(strcmp(tok, ">") == 0)
			{
				++g_than;
				if((g_than + gg_than) > 0)
				{
					cout << " Error: Invalid amount of output operators" << endl;
					exit(1);
				}
				output_file = strtok(NULL, " \t");
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
		
		//fdbefore = fdafter;
	
		fdbefore[0] = fdafter[0];
		fdbefore[1] = fdafter[1];

		//cout << "fdbefore[0]" << fdbefore[0] << endl;
		//cout << "fdbefore[1]" << fdbefore[1] << endl;
		
	
		if(i != (size - 1))
		{
			//cout << "piped" << endl;
			if(-1 == pipe(fdafter))
				perror("pipe");
		}
		//cout << "fdafter[0]" << fdafter[0] << endl;
		//cout << "fdafter[1]" << fdafter[1] << endl;
		//cout << "Called fork here " << endl;
		int pid = fork();
		if(pid == -1)
		{
			perror("fork");
		}
		else if(pid == 0) //child
		{
		//	cout << i << "!= 0" << endl;
			if(i != 0)
			{
				//cout<<"fdbefore[0] = stdin here" << endl;
				if(-1 == dup2(fdbefore[0],0))
					perror("dup2");
			}
		//	cout << i << " != " << size-1 << endl;
			if(i != (size - 1))
			{
				//cout << "fdafter[1] = stdout" << endl;
				if(-1 == dup2(fdafter[1],1))
					perror("dup2");
			}
			if(i == 0 && l_than == 0)
			{
				int in = open(input_file, O_RDONLY);
				if(in == -1)
					perror("open");
				if(-1 == close(0))
					perror("close");
				if(-1 == dup(in))
					perror("dup");
			}

			if(i == (size -1) && g_than == 0)
			{
				int out = open(output_file, O_WRONLY|O_CREAT, 0666);
				if(-1 == out)
					perror("open");
				if(-1 == close(1))
					perror("close");
				if(-1 == dup(out))
					perror("dup");

			}
			if(i == (size-1) && gg_than == 0)
			{
				int out = open(output_file, O_WRONLY|O_CREAT|O_APPEND);
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
		{
			wait(0);
		}

	}
	*/
	/*int k = 0;
	
	do
	{
	++k;
	if(-1 == wait(0))
	{
		perror("wait");
	}
	}
	while(k < size-1);
	*/
	/*for(int j = 0; j<size;++j)
	{
		if(-1 == wait(0))
			perror("wait");
	}*/
}

void cd(string commands)
{
	
}
void sighandler(int signum)
{
	return;
}

int main()
{

	signal(SIGINT,sighandler);
	while(true)
	{
		char *cur = new char[1024];
		if(NULL == getcwd(cur,1024))
		{
			perror("getcwd");
		}
		else
			cout << cur;
		delete[] cur;
		/*if(NULL == getlogin())//Display login user
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
		}*/
		
		string commands;//Receive command line
		cout << "$";
		getline(cin,commands);

		//Removing comment
		int size_str = commands.find("#");
		commands = commands.substr(0,size_str);
		

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
				else if(commands.find("|") != string::npos)
				{
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

					redirect(cmds, x);
					break;
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
					if(exec_v(curr)==true)
					next_it = false;
					commands = commands.substr(connect+2);
				}
				else if(flag == "&&")
				{
					string curr = commands.substr(0,connect);
					if(exec_v(curr)==false)
					next_it = false;
					commands = commands.substr(connect+2);
				}
				else if(flag == ";")
				{
					string next = commands.substr(0,connect);
					if(exec_v(next));
					commands = commands.substr(connect+1);
				}
				else
					exec_v(commands);
				flag = "";
			}
			else
			next_it = true;
		}
	
	}


	return 0;
}
