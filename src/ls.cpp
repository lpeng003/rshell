#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <iostream>

using namespace std;

void def_ls()
{
	struct stat buf;
	char* dirName =(char *) ".";
	DIR *dirp = opendir(dirName);
	dirent *direntp;
	cout << right;
	while ((direntp = readdir(dirp)))
	{
		if(direntp->d_name[0] == '.')
		{
			continue;
		}
		cout << direntp->d_name << "  "; // use stat here to find attributes of file
	}
	cout << endl;
	closedir(dirp);
}

void ls_a()
{
	struct stat buf;
	char* dirName = (char *)".";
	DIR *dirp = opendir(dirName);
	if(dirp == NULL)
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	while ((direntp = readdir(dirp)))
	{
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
		cout<< direntp->d_name << " "; // use stat here to find attributes of file
	}
	cout << endl;
	if(-1 == closedir(dirp))
	{
		perror("closeddir");
		exit(1);
	}
	
}

void flagl(dirent* direntp, struct stat buf)
{
	if(-1 == lstat(direntp->d_name, &buf))
	{
		perror("lstat");
		exit(1);
	}

	if(S_ISREG(buf.st_mode))
	{cout << "-";}
	else if(S_ISDIR(buf.st_mode))
	{cout << "d";}
	else if(S_ISCHR(buf.st_mode))
	{cout << "c";}
	else if(S_ISBLK(buf.st_mode))
	{cout << "b";}
	else if(S_ISFIFO(buf.st_mode))
	{cout << "p";}
	else if(S_ISLNK(buf.st_mode))
	{cout << "l";}
	else if(S_ISSOCK(buf.st_mode))
	{cout << "s";}

	//Usr permissions
	if(buf.st_mode & S_IRUSR) cout << "r";
	else cout << "-";
	if(buf.st_mode & S_IWUSR) cout << "w";
	else cout << "-";
	if(buf.st_mode & S_IXUSR) cout << "x";
	else cout << "-";
	
	//Grp permissions
	if(buf.st_mode & S_IRGRP) cout << "r";
	else cout << "-";
	if(buf.st_mode & S_IWGRP) cout << "w";
	else cout << "-";
	if(buf.st_mode & S_IXGRP) cout << "x";
	else cout << "-";

	//Other permissions
	if(buf.st_mode & S_IROTH) cout << "r";
	else cout << "-";
	if(buf.st_mode & S_IWOTH) cout << "w";
	else cout << "-";
	if(buf.st_mode & S_IXOTH) cout << "x";
	else cout << "-";
	
	cout << " ";
	cout << buf.st_nlink << " ";

	char *userid = getpwuid(buf.st_uid)->pw_name;
	cout << userid << " ";

	char *groupid = getgrgid(buf.st_gid)->gr_name;
	cout << groupid << " ";

	cout << buf.st_size << " ";
	
	time_t lm = buf.st_mtime;
	struct tm tim;
	if(localtime_r(&lm, &tim) == NULL)
	{
		perror("localtime_r");
		exit(1);
	}
	
	char brk_tm[100];
	strftime(brk_tm, sizeof(brk_tm), "%h",&tim);
	cout << brk_tm << " ";

	strftime(brk_tm, sizeof(brk_tm), "%d",&tim);
	cout << brk_tm << " ";

	strftime(brk_tm, sizeof(brk_tm), "%R",&tim);
	cout << brk_tm << " ";

	cout << direntp->d_name << endl;
	

}


void flagR(bool flagl, const char* dir)
{
	DIR *dirp = opendir(dir);
	if(dirp == NULL)
	{perror("opendir");}

	dirent* direntp;
	vector <char*> posdir;
	
	while((direntp = readdir(dirp)))
	{
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
		

	}

}


void ls_l(bool flagA)
{
	struct stat buf;
	char* dirName = (char *)".";
	DIR *dirp = opendir(dirName);
	if(dirp == NULL)
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	while ((direntp = readdir(dirp)))
	{
		if(!flagA)
		{
			if(direntp->d_name[0] == '.')
			{
				continue;
			}
		}
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
		
		
		flagl(direntp, buf);
	}
	if(-1 == closedir(dirp))
	{
		perror("closeddir");
		exit(1);
	}

}

void ls_R(bool flagA, bool flagl)
{
	struct stat buf;
	char* dirName = (char *)".";
	DIR *dirp = opendir(dirName);
	if(dirp == NULL)
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	cout << left;
	while ((direntp = readdir(dirp)))
	{
		if(!flagA)
		{
			if(direntp->d_name[0] == '.')
			{
				continue;
			}
		}
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
		if(-1 == stat(direntp->d_name,&buf))
		{
			perror("stat");
			exit(1);
		}
		if(S_ISDIR(buf.st_mode))
		{
			flagR(flagl, direntp->d_name);		
		}

	}
	if(-1 == closedir(dirp))
	{
		perror("closeddir");
		exit(1);
	}



}

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		def_ls();
	}
	bool flagA = false;
	bool flagl = false;
	bool flagR = false;
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
					{flagA = true;}
					if(flag_check[j] == 'l')
					{flagl = true;}
					if(flag_check[j] == 'R')
					{flagR = true;}
					++j;
				}
			}
			else
				break;//no more flags can be read in
		}	
	}
	//if(flagR)
	//{ls_R(flagA, flagl);}
	else if(flagl)
	{ls_l(flagA);}
	else if(flagA)
	{ls_a();}
	else
	{def_ls();}
	return 0;
}
