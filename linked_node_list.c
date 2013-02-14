#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

/*
 * Modified version. 
 * Original author: ad@di.uoa.gr 02/03
 */

struct item *CreateItem(struct item *p) {	
	p=NULL;
	return(p);
} // CreateItem

struct item* CreateItemWithData( char* courseId, char* courseName, char* grade ) {

	struct item * p = (struct item *) malloc( sizeof(struct item)+1 );
	strcpy( p->courseId, courseId );
  strcpy( p->courseName, courseName );
  strcpy( p->grade, grade );
	p->next = NULL;

	return p;
} // CreateItemWithData

struct item *InsertItem( struct item *p, struct item* item ) {

	if ( p == NULL ) {
		p = (struct item *) malloc( sizeof(struct item)+1 );
		strcpy( p->courseId, item->courseId );
  	strcpy( p->courseName, item->courseName );
  	strcpy( p->grade, item->grade );
		p->next = NULL;
	} else {
		p->next = InsertItem( p->next, item );
	}
	return p;
} // InsertItem


struct item* freeList( struct item* item ) {
  while ( item->next != NULL ) {
    item = freeList( item->next );
  }
  free( item );
  return NULL;
} // freeList


void PrintItem( struct item *p ) { 

	struct item *aux=NULL;

	if ( p == NULL ) {
		println("List is empty of items\n");
		return;
	}
	aux = p;
	while ( aux != NULL ) {
		println("Course: ( %s, %s, %s )", aux->courseId, aux->courseName, aux->grade);
		aux = aux->next;
	}

} // PrintItem
