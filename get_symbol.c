#include<stdio.h>

typedef enum
{S_WORD, S_BAR, S_AMP, S_SEMI, S_GT, S_GTGT, S_LT, S_NL, S_EOF} SYMBOL;

typedef enum
{NEUTRAL, GTGT, INQUOTE, INWORD} STATUS;

SYMBOL getsymb(char *word){
	// define variables 
	STATUS state;
	int c;
	char *w;

	status = NEUTRAL; //state initialization.
	w = word; //preserved the value of word at the end of the function.

	while((c=getchar()) != EOF){
		switch (state){
			case NEUTRAL: //because status is initialized NEUTRAL
				switch(c){
					case ';' :
						return S_SEMI;
					case '&' :
						return S_AMP;
					case '|' :
						return S_BAR;
					case '<' :
						return S_LT;
					case '\n' :
						return S_NL;

					//re-enter if blank or tab

					case ' ' : 
					case '\t' :
						continue;
					case '>' :
						state = GTGT;
						continue;
					case '"' :
						state = INQUOTE;
						continue;
					default :
						state = INWORD	
