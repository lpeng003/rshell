#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include "Timer.h"

using namespace std;

int main(int argc, char* argv[])
{
	if((argc == 1) || (argc == 2))
	{
		cout << "Error Usage: .exe input output" << endl;
		return 1;
	}

	int acs = access(argv[2], O_RDONLY);
	if(acs == 0)
	{
		perror("access");
		exit(1);
	}

	if(argc == 4)
	{
//below
		//Method 1
		Timer t1;
		double eTime1;
		t1.start();
		for (int i=0, j; i<1000000000; i++)
		{
			j++;
		}
		t1.elapsedWallclockTime(eTime1);
		cout << "Wallclock time: " << eTime1 << endl;
		t1.elapsedUserTime(eTime1);
		cout << "User time: " << eTime1 << endl;
		t1.elapsedSystemTime(eTime1);
		cout << "System time: " << eTime1 << endl;

		ifstream in(argv[1]);
		ofstream out(argv[2]);

		while(in.good())
		{
			char c = in.get();
			if(in.good())
			{
				out.put(c);
			}
		}
		

		
		//Method 2
		Timer t2;
		double eTime2;
		t2.start();
		for (int i=0, j; i<1000000000; i++)
		{
			j++;
		}
		t2.elapsedWallclockTime(eTime2);
		cout << "Wallclock time: " << eTime2 << endl;
		t2.elapsedUserTime(eTime2);
		cout << "User time: " << eTime2 << endl;
		t2.elapsedSystemTime(eTime2);
		cout << "System time: " << eTime2 << endl;


		int num = 0;
		char buf[1];
		
		int fdi = open(argv[1], O_RDONLY);
		if(fdi == -1)
		{
			perror("fdi");
			exit(1);
		}

		int fdo = open(argv[2], O_WRONLY|O_CREAT);
		if(fdo == -1)
		{
			perror("fdo");
			exit(1);
		}

		while(num = read(fdi, buf, 1))
		{
			if(num == -1)
			{
				perror("num");
				exit(1);
			}
			int w = write(fdo, buf, num);
			if(w == -1)
			{
				perror("w");
				exit(1);
			}
		}
		

		//Method 3
		Timer t3;
		double eTime3;
		t3.start();
		for (int i=0, j; i<1000000000; i++)
		{
			j++;
		}
		t3.elapsedWallclockTime(eTime3);
		cout << "Wallclock time: " << eTime3 << endl;
		t3.elapsedUserTime(eTime3);
		cout << "User time: " << eTime3 << endl;
		t3.elapsedSystemTime(eTime3);
		cout << "System time: " << eTime3 << endl;


		
		int num2 = 0;
		char buf2[BUFSIZ];
		
		int fdi2 = open(argv[1], O_RDONLY);
		if(fdi2 == -1)
		{
			perror("fdi");
			exit(1);
		}

		int fdo2 = open(argv[2], O_WRONLY|O_CREAT);
		if(fdo2 == -1)
		{
			perror("fdo");
			exit(1);
		}

		while(num2 = read(fdi2, buf2, BUFSIZ))
		{
			if(num2 == -1)
			{
				perror("num");
				exit(1);
			}
			int w = write(fdo2, buf2, num2);
			if(w == -1)
			{
				perror("w");
				exit(1);
			}
		}
//above
	}
	else
	{
		//Method 3
		/*
		Timer t3;
		double eTime3;
		t3.start();
		for (int i=0, j; i<1000000000; i++)
		{
			j++;
		}
		t3.elapsedWallclockTime(eTime3);
		cout << "Wallclock time: " << eTime3 << endl;
		t3.elapsedUserTime(eTime3);
		cout << "User time: " << eTime3 << endl;
		t3.elapsedSystemTime(eTime3);
		cout << "System time: " << eTime3 << endl;
		*/

		
		int num2 = 0;
		char buf2[BUFSIZ];
		
		int fdi2 = open(argv[1], O_RDONLY);
		if(fdi2 == -1)
		{
			perror("fdi");
			exit(1);
		}

		int fdo2 = open(argv[2], O_WRONLY|O_CREAT);
		if(fdo2 == -1)
		{
			perror("fdo");
			exit(1);
		}

		while(num2 = read(fdi2, buf2, BUFSIZ))
		{
			if(num2 == -1)
			{
				perror("num");
				exit(1);
			}
			int w = write(fdo2, buf2, num2);
			if(w == -1)
			{
				perror("w");
				exit(1);
			}
		}
	}
	return 0;
}
