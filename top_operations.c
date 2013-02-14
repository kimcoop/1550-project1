#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

void traverseLeaves( struct node* node ) {

	struct node* minNode = getMinNode( node );

	println("leftmost node has leftmost key %d", minNode->keys[0]); // min key in tree
	printKeysInNode( minNode );
	println("---");
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

int indexOfCourse( char* courseId, struct courseFreq* courseFreqs ) {

	return -1;

} // indexOfCourse

void getTopCourses( struct node* root, int top, int numInserts ) {
	struct courseFreq* courseFreqs[ numInserts ]; // array with <numInserts> pointers to courseFreq structs (account for max case)
	struct node* leafNode = getMinNode( root );
	struct item* course;
	char courseId[7];
	int i, j;

	while ( leafNode != NULL ) { // for each leafNode

		for ( i = 0; i < (leafNode->numChildren-i); i++ ) { // for each child in leafNode
			println("key[%d]= %d", i, leafNode->keys[i]);
			course = (struct item*) leafNode->courseList[i];
			j = 0;
			while ( course != NULL ) { // for each course in child
				println( "%s", course->courseName );
				j++;
				course = leafNode->courseList[j];
			}
		} // end for

		leafNode = leafNode->nextLeaf;
	} // end while



// struct courseFreq* courseFreq;

// struct courseFreq {
//   char courseId[7];
//   char courseName[8];
//   int freq;
// };

} // getTopCourses 










// getTopCourses
 /**************************

      topCoursesCollected = 0
      topCoursesArray = array[ top ] // track courseIDs (or courseNames)

      totalCoursesCollected = 0
      totalCoursesArray = array[ numInsertions ] // track all courses inserted. array of strings
      coursesCounterArray = array[ numInsertions ] // track num appearances for each course. array of ints
  
      while (totalCoursesCollected <= numInsertions) {
        traverse the tree from root down, just like for print, until leaf node
        if isLeafNode (node will have course data)
          for each item in courseList of node
            if indexOf item->courseId not in totalCoursesArray
              strcpy( totalCoursesArray[ totalCoursesCollected-1 ], item->courseId )
            else
              i = indexOf item->courseId in totalCoursesArray
              coursesCounterArray[ i ] ++ // increase frequency for course appearance
            totalCoursesCollected ++
      }

      i = 0
      while (topCoursesCollected <= top) {
        topCourseIndex = getIndexOgetIndexOfMaxFrequency( coursesCounterArray  ) // get most-enrolled-in course based on frequency
        topCourse = totalCoursesArray[ topCourseIndex ]
        topCoursesArray[ i ] = topCourse // most-enrolled-in course will be to the left of array
        totalCoursesArray[ topCourseIndex ] = null // set to null here
        coursesCounterArray[ topCourseIndex ] = -1 // prevent from being re-selected. let loop repeat
        i ++
        topCoursesCollected ++ // will be same as i
      }

      function getIndexOfMaxFrequency() {
        int currentMax = 0
        int indexOfMax = -1
        for (int i ... length) frequency in coursesCounterArray
          if frequency > currentMax
            currentMax = frequncy
            indexOfMax = i
        return i
      }

      */