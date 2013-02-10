#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

/*
 * This set of calls creates a linked list, and allows
 * for the removal of individual elements, printing out,
 * membership questions, and selective look-up (first, last)
 * element.
 * 				ad@di.uoa.gr 02/03
 */

struct item {
	char *word; //courseId
	struct item *next;
	struct courseData *courseData; //courseId[7], courseName[8], grade[3];
};


struct item *CreateItem(struct item *p)
{	
	printf("createItem\n");
	p=NULL;
	return(p);
}

struct item *InsertItem(struct item *p, char *w, struct courseData *data)
{
	if ( p==NULL ) {
		p = (struct item *) malloc (sizeof(struct item)+1);
		p->word = (char *) malloc(sizeof(w)+1);
		strcpy(p->word,w);
		p->courseData = data;
		p->next = NULL;
	} else {
		p->next = InsertItem(p->next, w, data);
	}
	return(p);
}


struct item *DeleteItem(struct item *p, char *w)
{	struct	item	*aux=NULL, *throwaway=NULL;

	aux=p;
	if (aux==NULL) 
		return (NULL);
	else {
		if ( strcmp(aux->word, w) == 0 ){
			throwaway=aux;
			aux = aux->next;
			free (throwaway) ;
			throwaway = NULL;
			return(aux);
			}
		else	{
			aux->next=DeleteItem(aux->next, w);
			return(aux);
			}
	}
}


char *GetFirstItem(struct item *p)
{       struct  item    *aux=NULL;
	char		*w=NULL;

	aux=p;
	if (aux==NULL)
		return(NULL);
	else	{
		w = (char *)malloc(sizeof(aux->word)+1);	
		strcpy(w, aux->word);
		printf("first word is %s \n",w);
		return(w);
		}
}
	
char *GetLastItem(struct item *p)
{	struct	item	*aux=NULL;
	char		*w=NULL;
	
	aux=p;
	if (aux==NULL)
		return(NULL);
	else	{
		while (aux->next!=NULL)
			aux=aux->next;
		w = (char *)malloc(sizeof(aux->word)+1);
                strcpy(w, aux->word);
                printf("last word is %s \n",w);
                return(w);
                }            
}

int	IsMemberOfList(struct item *p, char *w)
{	struct	item	*aux=NULL;
	
	aux = p;
	while (aux!=NULL){
		if ( strcmp( aux->word, w) == 0 )
			return(YES);
		aux=aux->next;
		}
	return(NO);
}


void PrintItem(struct item *p)
{ 
	struct item *aux=NULL;
	
	if ( p == NULL ){
		printf("List is Empty of Items\n");
		return;
		}
	aux = p;
	while (aux!= NULL){
		printf("Element courseId:  %s, grade:  %s\n", aux->word, aux->courseData->grade);
		aux=aux->next;
		}
}
