#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "my_shell.h"

#ifndef BADSIG
	#define BADSIG (void (*)) (-1)
#endif

#ifndef BSD
#ifndef OS2
	extern sys_nerr;
	extern char *sys_errlist[];
#endif
#endif

void fatal(message)
char *message;
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}


void syserr(message)
char *message;
{
	fprintf(stderr, "Error: %s (%d", message, errno);

	if (errno > 0 && errno < sys_nerr)
		fprintf(stderr, "; %s)\n", sys_errlist[errno]);
	else
		fprintf(stderr, ")\n");

	exit(1);
}

static void (* oldint)();
static void (* oldquit)();

void sigigno()
{
	static BOOLEAN first = TRUE;

	if (first) 
	{
		first = FALSE;
		oldint = signal(SIGINT, SIG_IGN);
		oldquit = signal(SIGQUIT, SIG_IGN);
		
		if (oldint == BADSIG || oldquit == BADSIG)
			syserr("signal");
	}
	else if (signal(SIGINT, SIG_IGN) == BADSIG ||
		signal(SIGQUIT, SIG_IGN) == BADSIG)

	syserr("signal");
}


void sigrest()
{
	if (signal(SIGINT, oldint) == BADSIG ||
		signal(SIGQUIT, oldquit) == BADSIG)		syserr("signal");
}



void waitfor(pid)
int pid;
{
	int wpid, status;

	while ((wpid = wait(&status)) != pid && wpid != ERROR)
		prnstate(wpid, status);

	if (wpid == pid)	prnstate(0, status);
}



#ifndef OS2

void prnstate(pid, status)
int pid;
int status;
{
	int k;

	if (status != 0 && pid != 0)
		printf("Process %d: ", pid);

	if (lowbyte(status) == 0) 
	{
		if ((k = highbyte(status)) != 0)
			printf("Exit code %d\n", k);
	}
	else 
	{
		if ((k = status & 0177) <= MAXSIG)
			printf("%s", sys_siglist[k]);
		else
			printf("Signal #%d", k);

		if ((status & 0200) == 0200)
			printf(" - core dumped");

		printf("\n");
	}
	return;
}
#endif




#ifdef OS2
void prnstate(pid, status)
int pid;
int status;
{
	int k;
	
	static char *sigmess[] = 
	{
		"",
		"Hangup",
		"Interrupt",
		"Quit",
		"Illegal instruction",
		"Trace trap",
		"IOT instruction",
		"EMT instruction",
		"Floating point exception",
		"Kill",
		"Bus error",
		"Segmentation violation",
		"Bad arg to system call",
		"Write on pipe",
		"Alarm clock",
		"Terminate signal",
		"User signal 1",
		"User signal 2",
		"Death of child",
		"Power fail"  };
		
		if (status != 0 && pid != 0)
			printf("Process %d: ", pid);

		if (lowbyte(status) == 0) 
		{
			if ((k = highbyte(status)) != 0)
				printf("Exit code %d\n", k);
		}
		else 
		{
			if ((k = status & 0177) <= MAXSIG)
				printf("%s", sigmess[k]);
			else
				printf("Signal #%d", k);

			if ((status & 0200) == 0200)
				printf(" - core dumped");
			printf("\n");
		}
	return;
}
#endif




BOOLEAN shellcmd(ac, av, sourcefd, destfd)
int ac;
char *av[];
int sourcefd;
int destfd;
{
	char *path;

	if (strchr(av[0], '=') != NULL)	envass(ac, av);
	else if (strcmp(av[0], "export") == 0)	envexport(ac, av);
	else if (strcmp(av[0], "set") == 0)		envset(ac, av);
	else if (strcmp(av[0], "cd") == 0) 
	{
		if (ac > 1)	path = av[1];
		else if ((path = envget("HOME")) == NULL)
			path = ".";
		if (chdir(path) == ERROR)
			fprintf(stderr, "%s: bad directory.\n", path);
	}else if (strcmp(av[0], "exit") == 0) 
	{
		exit(0);
	} else	return FALSE;

	if (sourcefd != 0 || destfd != 1)
		fprintf(stderr, "Ilegal redirection or pipeline.\n");

	return TRUE;
}
