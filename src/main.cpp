#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[])
{
	int pid = fork();
	if(pid == 0)//when pid is 0, this is the child process
	{
		cout << "This is the child.";
		exit(1);
	}
	else if(pid > 0)//When pid > 0, this is the parent process
	{
		if(-1 == wait(0))
		perror("There was a problem with wait");
		cout << "This is the parent.";
	}	
}
