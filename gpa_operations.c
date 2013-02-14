#include	<stdio.h>
#include 	<stdlib.h>
#include  <string.h>

void getGPA( struct node* root, int studentId ) {

	float gpa = calculateGPA( root, studentId );
	if ( gpa > 0 ) {
		println("GPA for studentId %d = %.2f. ", studentId, gpa);
	} else {
		println("ERROR:\nCould not calculate GPA for studentId %d.", studentId);
	}

} // getGPA

void getGPAinRange( struct node* root, int studentId_a, int studentId_b ) {
  
  struct nodeIndex* nodeIndexA = (struct nodeIndex*) nodeIndexForKey( root, studentId_a );
  struct nodeIndex* nodeIndexB = (struct nodeIndex*) nodeIndexForKey( root, studentId_b );

  int numStudentsInRange = 0;
  float sumAveragesGPA = 0.0;
  float averageGPA = 0.0;
  if ( studentId_a < studentId_b && nodeIndexA->wasFound && nodeIndexB->wasFound ) { // ensure both valid
  	int i = nodeIndexA->index; // position within node
  	struct node* node = (struct node*) nodeIndexA->node;
		struct item* courseList = (struct item*) nodeIndexA->node->courseList[i];

		while ( node != NULL && node->keys[i] <= studentId_b ) {
			sumAveragesGPA += calculateGPAofList( node->courseList[i] );
			numStudentsInRange++;
			i++;
			if ( i == 4 || i == node->numChildren ) { // reset when we jump to right leafnode
				i = 0;
				node = node->nextLeaf;
			}
		}

  	averageGPA = sumAveragesGPA / numStudentsInRange;
  	println("Average GPA for studentIds in range(%d..%d) = %.2f.", studentId_a, studentId_b, averageGPA);

  } else {
  	println("ERROR:\nBad studentId input for GPA (%d, %d).", studentId_a, studentId_b);
  }

} // getGPAinRange

float calculateGPA( struct node* root, int studentId ) {

  struct nodeIndex* nodeIndex = search( root, studentId );
	float gpa = 0.0;

	if ( nodeIndex->wasFound ) {

		int j = nodeIndex->index;

		if ( nodeIndex->node->courseList[ j ] != NULL ) {
			gpa = calculateGPAofList( nodeIndex->node->courseList[j] );
		} else {
			println("ERROR:\nFound studentId %d in tree, but has no children.", studentId);
		}

	} else {
		println("ERROR:\nStudentId %d was not found in tree.", studentId);
	}

	return gpa;
} // calculateGPA

float calculateGPAofList( struct item* p ) {
	// iterate through linkedlist items and sum grades

	int sumGrades = 0, gradesCount = 0;
	char grade[3];

	while ( p != NULL ) {
		strcpy( grade, p->grade );
		sumGrades += gradepointForGrade( grade );
		gradesCount++;
		p = p->next;
	}
	float gpa = sumGrades / gradesCount;
	return gpa;

} // calculateGPAofList


float gradepointForGrade( char* grade ) {
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
  else if ( (strEqual(grade, "F")) ) {  gpa = 0.0;  }
  else {  
  	println( "ERROR:\nGrade %s not recognized. Excluding from calculation.", grade );
  	gpa = 0.00;
  }

	return gpa;
} // gradepointForGrade