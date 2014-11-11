#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <iostream>

using namespace std;

void def_ls()
{
	struct stat buf;
	char* dirName = ".";
	DIR *dirp = opendir(dirName);
	dirent *direntp;
	cout << left;
	while ((direntp = readdir(dirp)))
	{
		if(direntp->d_name[0] == '.')
		{
			continue;
		}
		int stat_id = stat(direntp->d_name,&buf);
		if(stat_id==-1)
		{
			perror("stat");
			exit(1);
		}
		cout<< direntp->d_name << " "; // use stat here to find attributes of file
	}
	cout << endl;
	closedir(dirp);
}

void ls_a()
{
	struct stat buf;
	char* dirName = ".";
	DIR *dirp = opendir(dirName);
	dirent *direntp;
	cout << left;
	while ((direntp = readdir(dirp)))
	{
		int stat_id = stat(direntp->d_name,&buf);
		if(stat_id==-1)
		{
			perror("stat");
			exit(1);
		}
		cout<< direntp->d_name << " "; // use stat here to find attributes of file
	}
	cout << endl;
	closedir(dirp);
}


int main(int argc, char** argv)
{
	if(argc == 1)
	{
		def_ls();
	}
	bool flagA = true;
	bool flagl = true;
	bool flagR = true;
	//flags
	if(argc > 1)//Not default ls
	{
		for(int i = 1;i<argc;++i)
		{
			char* flag_check = argv[i];
			if(flag_check[0] == '-')
			{
				int j = 1;
				while(flag_check[j] != '\0')
				{
					if(flag_check[j] == 'a')
					{flagA = false;}
					if(flag_check[j] == 'l')
					{flagl = false;}
					if(flag_check[j] == 'R')
					{flagR = false;}
					++j;
				}
			}
		}	
	}
	if(!flagA)
	{
		ls_a();
	}
		
	return 0;
}
