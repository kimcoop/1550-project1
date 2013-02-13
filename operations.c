#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

int calculateGPA( struct node* root, int studentId ) {

  struct node* node = NULL;
	node = search( root, studentId );

	int sumGrades = 0, gradesCount = 0,  gpa;


	int i = 0;
	while ( node->keys[i] != studentId ) {
		i++;
	}
	println(" matching studentID in position %d", i );

	if ( node != NULL ) {
		int i=0;
		struct item *p = (struct item *) malloc( sizeof(struct item)+1 );
		char* s;

		if (node->courseList[i] != NULL) {
			PrintItem( node->courseList[i] );
			strcpy(s, node->courseList[i]->grade );
			println(" grade is %s", s );
			sumGrades += gradepointForGrade( s );
			gradesCount++;
		} else {
			println("oops null");
			return 0;
		}

		gpa  = sumGrades / gradesCount;
		println(" gpa is %d", gpa);
	}

	return gpa;
}


int gradepointForGrade( char* grade ) {
// expects grade as a string
// returns (float) int value for corresponding grade point

 if ( (strEqual(grade, "A+")) ) {
    println(" grade is A+");
 } else if ( (strEqual(grade, "A")) ) {
    println(" grade is A");
 } else if ( (strEqual(grade, "A-")) ) {
    println(" grade is A-");
 } else if ( (strEqual(grade, "B+")) ) {
    println(" grade is B+");
 } else if ( (strEqual(grade, "B")) ) {
    println(" grade is B");
 } else if ( (strEqual(grade, "B-")) ) {
    println(" grade is B-");
 } else {
 	 println( "Grade %s not recognized. ", grade );
 }

	return 0;
}