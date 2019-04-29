#include <stdio.h>
#include "my_shell.h"

main()
{

	char *prompt;

	int pid, fd;
	SYMBOL term;

	sigigno();

	if (!envinit())	fatal("cannot initialize environment.");

	if ((prompt = envget("PS2")) == NULL)	prompt = "> ";
	printf("%s", prompt);

	while (TRUE) {
		term = execcmd(&pid, FALSE, NULL);
		if (term != S_AMP && pid != 0)	waitfor(pid);
		if (term == S_NL)	printf("%s", prompt);

		for (fd=3; fd<MAXFD; fd++)	close(fd);
	}
}
