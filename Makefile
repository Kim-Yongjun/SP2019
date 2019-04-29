all:	my_shell

my_shell.o:	my_shell.c my_shell.h
	gcc -c my_shell.c

shellutil.o:	shellutil.c my_shell.h
	gcc -c shellutil.c

evset.o:	evset.c my_shell.h
	gcc -c evset.c

execsymb.o:	execsymb.c my_shell.h
	gcc -c execsymb.c

getsymb.o:	getsymb.c my_shell.h
	gcc -c getsymb.c

execcmd.o:	execcmd.c my_shell.h
	gcc -c execcmd.c

file_description.o:	file_description.c my_shell.h
	gcc -c file_description.c

my_shell:	my_shell.o shellutil.o evset.o execsymb.o getsymb.o execcmd.o file_description.o
	gcc -c my_shell my_shell.o shellutil.o evset.o execsymb.o getsymb.o execcmd.o file_description.o
