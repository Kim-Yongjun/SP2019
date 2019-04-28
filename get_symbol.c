#include<stdio.h>
#include<stdlib.h>

typedef enum
{S_WORD, S_BAR, S_AMP, S_SEMI, S_GT, S_GTGT, S_LT, S_NL, S_EOF} SYMBOL;

typedef enum
{NEUTRAL, GTGT, INQUOTE, INWORD} STATUS;

SYMBOL getsymb(char *word){
	// define variables 
	STATUS state;
	int c;
	char *w;

	state = NEUTRAL; //state initialization.
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
						state = INWORD;
						*w++ = c;
						continue;
				}
			case INQUOTE:
				switch (c){
					case '\\' : 
					       *w++ = getchar();
				       		continue;
				 	case '"' :
						*w = '\0';
						return S_WORD;
					default :
						*w++ = c;
						continue;
				}
			case INWORD:
				switch (c){
					case ';' :
					case '&' :
					case '|' : 
					case '<' :
					case '>' :
					case '\n' :
					case ' ' :
					case '\t' :
						ungetc(c,stdin);
						*w = '\0';
						return S_WORD;
					default:
						*w++ = c;
						continue;
				}
		}
	}
	return S_EOF;
}


int main(){
	char word[200];

	while (1){
		switch(getsymb(word)){
			case S_WORD:
				printf("S_WORD<%s>\n", word);
				break;
			case S_BAR:
				printf("S_BAR\n");
				break;
			case S_AMP:
				printf("S_AMP\n");
				break;
			case S_SEMI:
				printf("S_SEMI\n");
				break;
			case S_GT:
				printf("S_GT\n");
				break;
			case S_GTGT:
				printf("S_GTGT\n");
				break;
			case S_LT:
				printf("S_LT\n");
				break;
			case S_NL:
				printf("S_LN\n");
				break;
			case S_EOF:
				printf("S_EOF\n");
				exit(0);
		}
	return 0;
	}
}

































