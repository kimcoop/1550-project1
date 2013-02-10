#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

struct courseData {
	char courseId[7], courseName[8], grade[3];
	struct courseData *next;
};

struct courseData *CreateCourseList(struct courseData *p)
{	
	p=NULL;
	return(p);
}

struct courseData *InsertCourseData(struct courseData *p, char *courseId)
{
	if ( p==NULL ) {
		p = (struct courseData *) malloc (sizeof(struct courseData)+1);
		// p->courseId = (char *) malloc(sizeof(courseId)+1);
		strcpy(p->courseId, courseId);
		p->next = NULL;
		}
	else 	{
		p->next = InsertCourseData(p->next, courseId);
	}
	return(p);
}


struct courseData *DeleteCourseData(struct courseData *p, char *courseId) {	
	struct	courseData	*aux=NULL, *throwaway=NULL;

	aux=p;
	if (aux==NULL) 
		return (NULL);
	else {
		if ( strcmp(aux->courseId, courseId) == 0 ){
			throwaway=aux;
			aux = aux->next;
			free (throwaway) ;
			throwaway = NULL;
			return(aux);
			}
		else	{
			aux->next=DeleteCourseData(aux->next, courseId);
			return(aux);
			}
	}
}


char *GetFirstItem(struct courseData *p){       
	struct courseData *aux=NULL;
	char *w=NULL;

	aux=p;
	if (aux==NULL) {
		return(NULL);
	}	else {
		w = (char *)malloc(sizeof(aux->courseId)+1);
		strcpy(w, aux->courseId);
		printf("first courseId is %s \n",w);
		return(w);
		}
}
	
char *GetLastItem(struct courseData *p)
{	struct	courseData	*aux=NULL;
	char		*w=NULL;
	
	aux=p;
	if (aux==NULL)
		return(NULL);
	else	{
		while (aux->next!=NULL)
			aux=aux->next;
		w = (char *)malloc(sizeof(aux->courseId)+1);
                strcpy(w, aux->courseId);
                printf("last courseId is %s \n",w);
                return(w);
                }            
}

int	IsMemberOfList(struct courseData *p, char *courseId)
{	struct	courseData	*aux=NULL;
	
	aux = p;
	while (aux!=NULL) {
		if ( strcmp( aux->courseId, courseId) == 0 )
			return(YES);
		aux=aux->next;
		}
	return(NO);
}


void	PrintItem(struct courseData *p)
{ 
	struct courseData *aux=NULL;
	
	if ( p == NULL ){
		printf("List is Empty of Items\n");
		return;
		}
	aux = p;
	while (aux!= NULL){
		printf("Element %s\n",aux->courseId);
		aux=aux->next;
		}
}