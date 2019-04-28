#include <stdio.h>
#include <unistd.h>

int main()
{
	int pfd[2];
	
	if(pipe(pfd)==-1)		perror("pipe");
	switch(fork())
	{
		case	-1 : perror("fork");
		case	0	: 
			if(close(1)==-1)	perror("close");
			if(dup(pfd[1]) != 0)	perror("dup");
			if(close(pfd[0]) == -1 || close(pfd[1]) == -1)	perror("close2");
			execlp("ls", "ls", NULL);
			perror("execlp");
	}
	
	switch(fork())
	{
		case	-1 : perror ("fork");
		case	0	: 
			if(close(0) == -1)	perror("close3");
			if(dup(pfd[0]) != 0)	perror("dup2");
			if(close(pfd[0]) == -1 || close(pfd[1]) == -1)	perror("close4");
			execlp("more", "more", NULL);
			perror("execlp");
	}
	
	if(close(pfd[0]) == -1 || close(pfd[1]) == -1)	perror("close5");
	while(wait(NULL) != -1);
	return 0;
}
