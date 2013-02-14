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

int indexOfCourseId( char* courseId, struct courseFreq* courseFreqs ) {

	// int i = 0;
	// struct courseFreq* courseFreq = (struct courseFreq*) courseFreqs[i];
	// while ( courseFreq != NULL && ( strcmp(courseFreq->courseId, courseId) != 0) ) {
	// 	println(" courseFreq->courseId %s != %s", courseFreq->courseId, courseId );
	// 	i++;
	// 	courseFreq = (struct courseFreq*) courseFreqs[i];
	// }
	// println(" out of while ");
	// return ( courseFreq == NULL ? -1 : i );
	return -1;

} // indexOfCourseId

void getTopCourses( struct node* root, int top, int numInserts ) {
	struct courseFreq* courseFreqs[ numInserts ]; // array with <numInserts> pointers to courseFreq structs (account for max case)
	struct node* leafNode = getMinNode( root );
	struct item* course;
	// char courseId[7];
	int i;
	int numCourses = 0;

	while ( leafNode != NULL ) { // for each leafNode

		for ( i = 0; i < leafNode->numChildren; i++ ) { // for each child in leafNode
			println("key[%d]= %d", i, leafNode->keys[i]);
			course = (struct item*) leafNode->courseList[i];
			while ( course != NULL ) { // for each course in child
				// int courseIndex = indexOfCourseId(course->courseId, courseFreqs);
				println(" searching for course with courseId %s", course->courseId );
				int k = 0;
				if ( courseFreqs != NULL ) {
					struct courseFreq* courseFreq = (struct courseFreq*) courseFreqs[k];
					println(" courseFreq[0]->courseName = %s", courseFreq->courseName );
					println(" courseFreq->courseId != NULL ? %d", courseFreq->courseId != NULL );
					while ( courseFreq->courseId != NULL && k < numInserts && ( strcmp(courseFreq->courseId, course->courseId) != 0) ) {
						println(" courseFreq->courseId %s != %s", courseFreq->courseId, course->courseId );
						k++;
						courseFreq = (struct courseFreq*) courseFreqs[k];
					}
					println(" out of while ");
					int courseIndex = ( courseFreq == NULL ? -1 : k );


					if ( courseIndex != -1 ) { // courseId was found in array
						courseFreqs[ courseIndex ]->freq = courseFreqs[ courseIndex ]->freq + 1;
						println("courseId %s was found in array", course->courseId);
					} else {
						struct courseFreq* newCourseFreq = (struct courseFreq*) malloc( sizeof(struct courseFreq)+1 );
						strcpy( newCourseFreq->courseId, course->courseId);
						strcpy( newCourseFreq->courseName, course->courseName);
						newCourseFreq->freq = 1;
						courseFreqs[ numCourses ] = newCourseFreq;
						numCourses++;	
						println("courseId %s was NOT FOUND in array. numCourses is %d", course->courseId, numCourses);
					}
				} // end if 

				println( "%s", course->courseName );
				course = course->next;
			} // end while 
		} // end for
		println("--jumping to nextLeaf--");

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