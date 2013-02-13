/*
Kim Cooperrider
kac162
cs1550
Project 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "MyHeader.h"
#include "LinkedList.c"
#include "operations.c"

void printNode( struct node* node ) { // Print the node
  if ( node != NULL ) {
    println(">> NODE: numChildren: %d, isLeafNode?: %d", node->numChildren, node->isLeafNode);
    println(">> node's keys 0..%d:", node->numChildren);
    int i;
    for ( i = 0; i < node->numChildren; i++ ) {
      println(">> keys[%d]=%d, \t", i, node->keys[i]);
      if ( node->isLeafNode == YES ) { // then has linkedList  
        PrintItem( node->courseList[i] );
      }
    }
  } else {
    println(" OOOPS NODE IS NULL ***" );
  }
} // printNode

void printTree( struct node* root ) {
  println("printTree");
  printNode( root );
  if ( root->numChildren > 0 ) {
    println(" children ");
    int i;
    for ( i = 0; i < root->numChildren; i++ ) {
      println( "i is %d", i );
      printNode( root->children[i] );
    }
  }
} // printTree

struct node* createTree( void ) { // return a pointer to the root node
  struct node* root = (struct node*) malloc( sizeof(struct node)+1 );
  root->isLeafNode = YES;
  root->numChildren = 0;
  // int i;
  // for ( i = 0; i < 4 ; i++ ) {
  //   root->courseList[i] = (struct item*) (malloc( sizeof(struct item)+1 ));
  // }

  return root;
} // init

struct node* search( struct node* node, int studentId ) {

  println(" search");
  int i = 0;
  while ( i < node->numChildren && i < 5 && studentId > node->keys[i] ) {
    i++;
  }
  if ( i < node->numChildren && studentId == node->keys[i] ) { // studentId exactly matches node's child at i
    println("search results: %d FOUND", studentId);
    return node; // return (node, i)
  } else if ( node->isLeafNode == YES ) { // if we hit a leaf node, we've run out
    println("search results: %d NOT FOUND  ", studentId);
    return NULL;
  } else {
    printNode ( node );
    println(" ##\n i = %d", i );
    printNode ( node->children[i] );
    return search( node->children[i], studentId ); // return
  }

} // search

struct node* split( struct node* node, int i  ) {

  struct node* z = (struct node*) malloc( sizeof(struct node) );

  struct node* y = (struct node*) node->children[i]; // y is node's leftmost child
  int median = y->numChildren / 2; // median is half the number of y's children
  z->numChildren = y->numChildren - median; // equally divide y's children (take median number)
  z->isLeafNode = y->isLeafNode;
  println("NODE passed into split: ");
  printNode( node );
  println("NODE Z: (node we're constructing):");
  printNode( z );
  println("NODE Y (node's child at %d ):", i);
  printNode( y );

  int j;
  for ( j=0; j < median; j++ ) {
    z->keys[j] = y->keys[j+median];
  }

  if ( !y->isLeafNode ) {
    for ( j=0; j < median; j++ ) {
      z->children[j] = y->children[j+median];
      z->courseList[j] = y->courseList[j+median];
    } 
  }

  y->numChildren = median;
  for ( j=node->numChildren; j >= i; j-- ) {
    node->children[j] = node->children[i];
    node->courseList[j] = node->courseList[i];
    node->keys[j] = node->keys[i];
  }

  node->children[i+1] = z; // plop new node into place
  node->keys[i] = y->keys[median-1];
  node->courseList[i] = y->courseList[median-1];
  node->numChildren = node->numChildren+1;
  println("ROOT:");
  printNode(node);
  println("Y NODE:");
  printNode(y);
  println("Z NODE:");
  printNode(z);

  return node;

} // split

struct node* insertNonfull( struct node* node, int studentId, struct item* item ) {
  println("insertNonfull");
  int i = node->numChildren;
  if ( node->isLeafNode == YES ) {
    while ( i > 0 && studentId < node->keys[i-1] ) { // while we have at least one child node and the studentId is less than current key
      node->keys[i] = node->keys[i-1]; // shift keys right by one node
      i--;
    }
    node->keys[i] = studentId; // slot studentId in the right place
    node->courseList[i] = InsertItem( node->courseList[i], item );
    node->numChildren = node->numChildren + 1;
  } else {
    while ( i > 0 && studentId < node->keys[i-1] ) { // while studentId < node's greatest key
      i--;
    }
    if ( node->children[i]->numChildren == 4 ) { // if the child node at index where studentId should go has no free slots,
      node = split( node, i );                  // split the node
      if ( studentId > node->keys[i] ) { // if studentId greater than node keys[i]
        i++;
      }
    }
    node = insertNonfull( node->children[i], studentId, item ); // insert studentId into node's ith child (nonfull) // TODO - why is this i+1??
  }

  return node;
} // insertNonfull

struct node* insertMax( struct node* node, int studentId, struct item* item ) {
  println(" insertMax ");
  int i = node->numChildren;
  node->keys[i] = studentId; // node's greatest key is studentId
  if ( node->isLeafNode == YES ) {
    node->courseList[i] = InsertItem( node->courseList[i], item );
    node->numChildren = node->numChildren + 1;
  } else {
    if ( node->children[i-1]->numChildren == 4 ) { // if slotting in studentId made 4 children
      node = split( node, i );
      i++;
    }
    node = insertMax( node->children[i-1], studentId, item );
  }

  return node;
} // insertMax

struct node* insertData( struct node* root, int studentId, char* courseId, char* courseName, char* grade ) { // char courseId[7], courseName[8], grade[3];

  if ( studentId > 0 ) {
    struct item* item = CreateItemWithData( courseId, courseName, grade );
    println( "item created." );
    printNode( root );
    root = insert( root, studentId, item );
    return root;
  } else {
    println("--error--\nMalformed studentId. Is there a trailing newline at the end of your input file?");
    exit(0);
  }

} // insertData

struct node* insert( struct node* root, int studentId, struct item* item ) {
// returns root node
  println(" insert. root->children = %d ", root->numChildren);
  int i = root->numChildren;
  if ( i == 4 ) { // if node has no free slots
    struct node* node = (struct node*) malloc( sizeof(struct node)+1 ); // allocate new node
    node->isLeafNode = NO;
    node->numChildren = 1;
    node->children[0] = root; // new node's first child is root
    node->keys[0] = root->keys[3]; // first child of new node is greatest child of root
    root = node;
    node = split( node, 0 );
    if ( studentId > node->keys[1] ) {
      node = insertMax( node, studentId, item );
    } else {
      node = insertNonfull( node, studentId, item );
    }
  } else {
    int j = i != 0 ? i-1 : 0;
    if ( studentId > root->keys[j] ) { // if studentId > the the node's greatest child
      root = insertMax( root, studentId, item ); // then studentId is the max for this node
    } else {
      root = insertNonfull( root, studentId, item ); // else insert amidst the children nodes
    }
  }

  return root;
}// insert

void freeNode( struct node* node ) {
  if ( node != NULL && node->numChildren > 0 ) {
    int i;
    for (i = 0; i < node->numChildren; i++) {
      freeNode( node->children[i] );
    }
    free( node );
  }
}

void freeTree( struct node* root ) {
  println( "freeingTree ");
  if ( !root->isLeafNode && root->numChildren > 0 ) {
    int i;
    for ( i = 0; i < root->numChildren; i++ ) {
      freeNode( root->children[i] );
    }
  }
  println(" freeing root node ");
  free( root );
} // freeTree

int loadFile( struct node* root, char* filename ) {

  struct item *courseList = NULL;
  FILE  *fp = NULL;
  char separator;
  int numInsertions = 0;
  if ( (fp = fopen( filename, "r" )) == NULL ) {
    println("Unknown file");
    exit(1);
  } else {
    while ( !feof(fp)  ) { // studentId > 0 protects against trailing newline in file
      int studentId = 0;
      char courseId[7], courseName[8], grade[3];
      fscanf( fp, "%d %s %s %s", &studentId, courseId, courseName, grade );
      fscanf( fp, "%c", &separator );
      root = insertData( root, studentId, courseId, courseName, grade );
      if ( studentId != 0) numInsertions++;
      studentId = 0; // reset
    } // end while 
    fclose(fp);
  }

  println(" returning numInsertions %d", numInsertions );
  return numInsertions;
} // loadFile

int main( int argc, char *argv[] ) {
  
  struct node* root = NULL;
  root = createTree(); // points to root node of tree
  int execute = YES; // while loop control
  int numInsertions = 0;

  if ( argc != 2 ) {
    println("No filename provided." );
  } else {
    int numInsertionsFromFile = loadFile( root, argv[1] );
    numInsertions += numInsertionsFromFile;
  }

  while ( execute ) {
    char cmd[MAX_COMMAND_SIZE];
    int studentId, studentId_a, studentId_b, top;
    char courseId[7], courseName[8], grade[3];
    char filename[MAX_COMMAND_SIZE];

    printf("Please enter your action: ");
    scanf("%s", cmd);

    if ( strEqual(cmd, "exit") ) {
      execute = NO;

    } else if ( strEqual(cmd, "printtree") ) {
      printTree( root );

    } else if ( strEqual(cmd, "printroot") || strEqual(cmd, "print") ) {
      printNode( root );
    
    } else if ( strEqual(cmd, "find") ) {
      scanf("%d", &studentId);
      search( root, studentId );
    
    } else if ( strEqual(cmd, "ins") ) { // studentId, courseId, courseName, grade
      scanf("%d %s %s %s", &studentId, courseId, courseName, grade);
      root = insertData( root, studentId, courseId, courseName, grade); // insert new data
      if ( studentId > 0 ) numInsertions++;
    
    } else if ( strEqual(cmd, "load") ) { // filename
      scanf("%s", filename);
      int numInsertionsFromFile = loadFile( root, filename );
      numInsertions += numInsertionsFromFile;
    
    } else if ( strEqual(cmd, "range") ) { // studentId_a, studentId_b
      scanf("%d %d", &studentId_a, &studentId_b);
      println("range");
    
    } else if ( strEqual(cmd, "gpa") ) { // gpa <studentId> or gpa<studentId_a> <studentId_b> (range) 
      scanf("%d", &studentId); // for now, assume we have one studentId input
      println("finding GPA for studentId %d", studentId);
      int gpa = calculateGPA( root, studentId );
      println( "GPA for student with id=%d is %d", studentId, gpa );
    
    } else if ( strEqual(cmd, "top") ) {
      scanf("%d", &top);
      println(" calculating top %d popular courses", top);
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

    } else if ( strEqual(cmd, "verify") ) { // check all nodes to ensure properties of 2-4 tree
      println("verify");
    }

    strlcpy( cmd, "", sizeof( cmd ) );

  } // end while

  freeTree( root );
 
  return 0;
 
}
