#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PATH	100
#define LF		10

int print_prompt(){
	char *buffer = (char *)malloc(MAX_PATH);
	char *value = (char *)getcwd (buffer, MAX_PATH); //getcwd is similar to pwd in shell

	if(value != 0)
		fprintf(stdout, "yj_shell:%s>", buffer);

	free(buffer);

	return 0;
}

int main(){
	char c;
	while((c = getchar()) != EOF){
		if(c==LF)
			print_prompt();
	}
	return 0;
}


