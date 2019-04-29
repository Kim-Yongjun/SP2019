#define	ERROR	(-1)
#define	MAXFNAME	10
#define	MAXARG	10
#define	MAXWORD	20
#define	MAXFD	20
#define	MAXVAR	50
#define	MAXNAME	20

#ifdef	MAXSIG
	#define	MAXSIG	19
#endif

#define TRUE	1
#define FALSE	0

#define	BADFD	(-2)
#define	lowbyte(w)	((w) & 0377)
#define	highbyte(w)	lowbyte((w) >> 8)

typedef int BOOLEAN;
typedef enum {S_WORD, S_BAR, S_AMP, S_SEMI, S_GT, S_GTGT,
				S_LT, S_NL, S_EOF} SYMBOL;

SYMBOL execcmd();
int exsimcmd();
void fdredir();
char *envget();
int envinit();
BOOLEAN envupdate();
void envprint();
void envexport();
void envass();
void envset();
void sigigno();
void sigrest();
void fatal();
void syserr();
void prnstate();
