#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

float calculateGPA( struct node* root, int studentId ) {

  struct node* node = NULL;
	node = search( root, studentId );

	int sumGrades = 0, gradesCount = 0;
	float gpa = 0.0;

	if ( node != NULL ) {

		int i = 0;
		while ( node->keys[i] != studentId ) {
			i++;
		}
		println(" matching studentID in position %d", i );

	
		int j=0;
		struct item *p = (struct item *) malloc( sizeof(struct item)+1 );
		char grade[3];

		if (node->courseList[j] != NULL) {
			PrintItem( node->courseList[j] );
			strcpy(grade, node->courseList[j]->grade );
			sumGrades += gradepointForGrade( grade );
			gradesCount++;
		} else {
			println("oops null");
			return 0;
		}

		gpa  = sumGrades / gradesCount;
	}

	return gpa;
}


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


















