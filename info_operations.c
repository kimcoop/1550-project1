#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>


void getInfoInRange( struct node* root, int studentId_a, int studentId_b ) {

  struct nodeIndex* nodeIndexA = (struct nodeIndex*) nodeIndexForKey( root, studentId_a );
  struct nodeIndex* nodeIndexB = (struct nodeIndex*) nodeIndexForKey( root, studentId_b );

  if ( studentId_a < studentId_b && nodeIndexA->wasFound && nodeIndexB->wasFound ) { // ensure both valid
  	int i = nodeIndexA->index; // position within node
  	int numCourses = 0;
  	struct node* node = (struct node*) nodeIndexA->node;
		struct item* courseList = (struct item*) nodeIndexA->node->courseList[i];

		while ( node != NULL && node->keys[i] <= studentId_b ) {
			println("Student with id %d: ", node->keys[i]);
			getGPA( root, node->keys[i]);
			printCoursesInList( node->courseList[i] );
			i++;
			numCourses++;
			if ( i == 4 || i == node->numChildren ) { // reset when we jump to right leafnode
				i = 0;
				node = node->nextLeaf;
			}
			println("----");
		}

  } else {
  	println("ERROR:\nBad studentId input for range (%d, %d).", studentId_a, studentId_b);
  }

} // getInfo


void printCoursesInList( struct item* course ) {

	while ( course != NULL ) {
		PrintItem( course );
		course = course->next;
	} // end while

} // printCoursesInList