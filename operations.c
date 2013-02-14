#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

float calculateGPA( struct node* root, int studentId ) {

  struct nodeIndex* nodeIndex = search( root, studentId );
	float gpa = 0.0;

	if ( nodeIndex->wasFound ) {

		int sumGrades = 0, gradesCount = 0;
		int i, j;
		char grade[3];
		j = nodeIndex->index;

		if ( nodeIndex->node->courseList[ j ] != NULL ) {
			struct item *p = nodeIndex->node->courseList[ j ];

			// iterate through linkedlist items and sum grades
			while ( p != NULL ) {
				strcpy(grade, p->grade );
				println(" sumGrades += p->grade (%s)", grade );
				sumGrades += gradepointForGrade( grade );
				gradesCount++;
				p = p->next;
			}
			gpa  = sumGrades / gradesCount;

		} else {
			println("Oops. Found studentId %d in tree, but has no children.", studentId);
		}

	} else {
		println("Oops. StudentId %d was not found in tree.", studentId);
	}

	return gpa;
} // calculateGPA


float gradepointForGrade( char* grade ) {
// expects grade as a string
// returns float value for corresponding grade point
float gpa = 0.00;

 if ( (strEqual(grade, "A+")) ) {  gpa = 4.00; } 
  else if ( (strEqual(grade, "A"))  ) { 	gpa = 4.00; } 
  else if ( (strEqual(grade, "A-")) ) {  gpa = 3.70;  }
  else if ( (strEqual(grade, "B+")) ) {  gpa = 3.33;  }
  else if ( (strEqual(grade, "B"))  ) {  gpa = 3.00;  }
  else if ( (strEqual(grade, "B-")) ) {  gpa = 2.70;  }
  else if ( (strEqual(grade, "C+")) ) {  gpa = 2.30;  }
  else if ( (strEqual(grade, "C"))  ) {  gpa = 2.00;  }
  else if ( (strEqual(grade, "C-")) ) {  gpa = 1.70;  }
  else if ( (strEqual(grade, "D+")) ) {  gpa = 1.30;  }
  else if ( (strEqual(grade, "D"))  ) {  gpa = 1.00;  }
  else if ( (strEqual(grade, "D-")) ) {  gpa = 0.70;  }
  else {  
  	println( "Grade %s not recognized. ", grade );
  	gpa = 0.00;
  }

	return gpa;
}


















