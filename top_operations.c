#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

void traverseLeaves( struct node* node ) {
	printKeysInNode( getMinNode( node ) );
} // traverseLeaves

struct node* getMinNode( struct node* node ) {

	int i = 0;
	while ( !node->isLeafNode && node->numChildren > 0 ) {
		node = node->children[0]; // traverse through leftmost node
	}

	return node;

} // getMinNode

void printKeysInNode( struct node* node ) {

	if ( node != NULL ) {
		int i;
		for ( i=0; i < node->numChildren; i++ ) { // scrape node for keys then go to nextLeaf
			if (i==0) printf("(  ");
			printf("%d", node->keys[i]);
			if (i >= 0 && i<node->numChildren-1) printf(", ");
			if (i==node->numChildren-1) printf(" )");
		}
		printf("\n");
		printKeysInNode( node->nextLeaf );
	}

} // printKeysInNode

void getTopCourses( struct node* root, int top, int numInserts ) {
	println("STUB: Calculating top %d most popular courses", top);
	return;

	struct courseFreq* courseFreqs[ numInserts ]; // array with <numInserts> pointers to courseFreq structs (account for max case)
	struct node* leafNode = getMinNode( root );
	struct item* course;
	int i;
	int numCourses = 0;

	while ( leafNode != NULL ) { // for each leafNode

		for ( i = 0; i < leafNode->numChildren; i++ ) { // for each child in leafNode
			course = (struct item*) leafNode->courseList[i];
			int courseIndex = -1;
			while ( course != NULL ) { // for each course in child
				int k = 0;

				if ( numCourses > 0 ) { // course cannot be found in empty array
					struct courseFreq* courseFreq = (struct courseFreq*) courseFreqs[k];
					while ( courseFreq != NULL && k < numCourses && ( strcmp(courseFreq->courseId, course->courseId) != 0) ) {
						courseFreq = (struct courseFreq*) courseFreqs[k+1];
						k++;
					}
					courseIndex = ( courseFreq == NULL ? -1 : k );
				} // if

				if ( courseIndex > -1 ) { // courseId was found in array
					struct courseFreq* courseFreq = (struct courseFreq*) courseFreqs[ courseIndex ];
					courseFreq->freq = courseFreq->freq + 1;
				} else {
					struct courseFreq* newCourseFreq = (struct courseFreq*) malloc( sizeof(struct courseFreq)+1 );
					strcpy( newCourseFreq->courseId, course->courseId);
					strcpy( newCourseFreq->courseName, course->courseName);
					newCourseFreq->freq = 1;
					courseFreqs[ numCourses ] = newCourseFreq;
					numCourses++;	
				}

				course = course->next;
			} // end while
		} // end for
		leafNode = leafNode->nextLeaf;
	} // end while
} // getTopCourses 




