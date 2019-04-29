#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "minish.h"

extern char **environ;

static struct slot {
	char *name;
	char *value;
	BOOLEAN export;
} envsym[MAXVAR];

static struct slot *slfind(name)
char *name;
{
	int i;
	struct slot *v;
	
	v = NULL;

	for (i = 0; i < MAXVAR; i++)
		if (envsym[i].name == NULL) 
		{
			if (v == NULL) v = &envsym[i];
		}
		else if (strcmp(envsym[i].name, name) == 0) 
		{
			v = &envsym[i];
			break;
		}

	return v;
}


static BOOLEAN assign(p, s)
char **p;
char *s;
{
	int size;

	size = strlen(s) + 1;

	if (*p == NULL) 
	{
		if ((*p = (char *) malloc(size)) == NULL)
			return FALSE;
	}
	else if ((*p = (char *) realloc(*p, size)) == NULL)
		return FALSE;

	strcpy(*p, s);
	return TRUE;
}

BOOLEAN set(name, value)
char *name;
char *value;
{
	struct slot *v;
	BOOLEAN b;

	if ((v = slfind(name)) == NULL)
		return FALSE;

	b = assign(&v->name, name) && assign(&v->value, value);
	return b;
}

BOOLEAN export(name)
char *name;
{
	struct slot *v;

	if ((v = slfind(name)) == NULL)	return FALSE;

	if (v->name == NULL)
		if (!assign(&v->name, name) || !assign(&v->value, ""))
			return FALSE;

	v->export = TRUE;
	return TRUE;
}


char *envget(name)
char *name;
{
	struct slot *v;

	if ((v = slfind(name)) == NULL || v-> name == NULL)
		return NULL;

	return v->value;
}


BOOLEAN envinit()
{
	int i, leng;
	char name[MAXNAME];

	for (i = 0; environ[i] != NULL; i++) 
	{
		leng = strcspn(environ[i], "=");
		strncpy(name, environ[i], leng);
		name[leng] = '\0';
		
		if (!set(name, &environ[i][leng+1]) || !export(name))
			return FALSE;
	}
	return TRUE;
}


void envass(ac, av)
int ac;
char *av[];
{
	char *name, *value;
	if (ac != 1)	printf("Extra args.\n");
	else 
	{
		name = strtok(av[0], "=");
		value = strtok(NULL, "\1"); /* alles uebrige */
		if (!set(name, value))	fprintf(stderr, "Cannot set.\n");
	}
}


void envexport(ac, av)
int ac;
char *av[];
{
	int i;

	if (ac == 1) 
	{
		envset(ac, av);
		return;
	}

	for (i = 1; i < ac; i++)
		if (!export(av[i])) 
		{
			fprintf(stderr, "Cannot export %s\n", av[i]);
			return;
		}
}


void envset(ac, av)
int ac;
char *av[];
{
	if (ac != 1)	printf("Extra args.\n");
	else			envprint();

	return;
}


void envprint()
{
	int i;

	for (i = 0; i < MAXVAR; i++)
		if (envsym[i].name != NULL)
			printf("%3s %s=%s\n", envsym[i].export ? "[E]" : "",
					envsym[i].name, envsym[i].value);

	return;
}


BOOLEAN envupdate()
{
	int i, envi, nvlen;
	struct slot *w;

	static BOOLEAN updated = FALSE;

	if (!updated)
		if ((environ = (char **) malloc((MAXVAR+1) *
			sizeof(char *))) == NULL)
		return FALSE;

	envi = 0;

	for (i = 0; i < MAXVAR; i++) 
	{
		w = &envsym[i];

		if (w->name == NULL || !w->export)	continue;		
		nvlen = strlen(w->name) + strlen(w->value) + 2;

		if (!updated) 
		{
			if ((environ[envi] = (char *) malloc(nvlen)) == NULL)
				return FALSE;
		} else 
			if ((environ[envi] = (char *) realloc(environ[envi], nvlen)) == NULL)
				return FALSE;
		sprintf(environ[envi], "%s=%s", w->name, w->value);
		envi++;   
   }

   environ[envi] = NULL;
   updated = TRUE;
   return TRUE;
}
