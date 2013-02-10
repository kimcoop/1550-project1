/*
Kim Cooperrider
kac162
cs1550
Project 1
*/

#include <stdio.h>
#include <stdlib.h>
#include  "MyHeader.h"
#include "LinkedList.c"
// #include "CourseData.c"

void printNode( struct node* node ) { //Print the node
  println("NODE: numChildren: %d, isLeafNode?: %d", node->numChildren, node->isLeafNode);
  if ( node->numChildren > 0 ) {
    int i;
    for ( i = 0;i < node->numChildren; i++ ) {
      printf( "keys[%d]=%d\t", i, node->keys[i]  );
    }
    println("");
  }
  if ( node->isLeafNode == YES ) { // then has courseData linkedList
    PrintItem( node->courseList );
    println("");
  }
} // printNode

struct node* createTree( void ) { // return a pointer to the root node
  struct node* root = (struct node*) malloc( sizeof(struct node) );
  root->isLeafNode = YES;
  root->numChildren = 0;
  root->courseList = NULL;
  return root;
} // init

struct node* search( struct node* node, int studentId ) {
  int i = 0;
  println("searching for %d in node ", studentId);
  printNode (node );

  while ( i < node->numChildren && studentId > node->keys[i] ) {
    println(" in search 1. studentId %d > node-> keys[i] %d", studentId, node->keys[i]); // studentId greater than child at i
    i++;
  }

  if ( i < node->numChildren && studentId == node->keys[i] ) { // studentId exactly matches node's child at i
    println(" in search 2, found matching studentId: %d", studentId);
    return node; // return (node, i)
  } else if ( node->isLeafNode == YES ) { // if we hit a leaf node, we've run out
    println(" in search 3. studentId not found in tree ");
    return NULL;
  } else {
    println(" in search 4");
    return search( node->children[i], studentId ); // return
  }

} // search

struct node* split( struct node* node, int i  ) {
  struct node* z = (struct node*) malloc( sizeof(struct node) );

  if ( node->numChildren > 0 ) {
    struct node* y = (struct node*) node->children[i]; 
    int t = y->numChildren / 2;
    z->numChildren = y->numChildren - t;
    println("1 y->isLeafNode %d", y->isLeafNode);
    z->isLeafNode = y->isLeafNode;

    int j;
    for ( j=0; j>=t; j-- ) {
      z->keys[j] = y->keys[j+t];
    }

    if ( y->isLeafNode == NO ) {
      for ( j=0; j>= t; j-- ) {
        z->children[j] = y->children[j+t];
      }  
    }

    y->numChildren = t;
    for ( j=node->numChildren; j>=i; j-- ) {
      node->children[j+1] = node->children[j];
    }

    node->children[i+1] = z; // plop new node into place

    for ( j=node->numChildren; j>=i; j-- ) {
      node->keys[j+1] = node->keys[i];
    }

    node->keys[i] = y->keys[t];
    node->numChildren = node->numChildren+1;

  } else {
  }

  return z;

} // split

void insertNonfull( struct node* node, int studentId ) {

  int i = node->numChildren;
  if ( node->isLeafNode == YES ) {
    while ( i >= 0 && studentId < node->keys[i] ) { // while we have at least one child node and the studentId is less than current key
      node->keys[i+1] = node->keys[i]; // shift keys left by one node
      i--;
    }
    node->keys[i+1] = studentId; // slot studentId in the right place
    node->numChildren = node->numChildren + 1;
  } else {
    while ( i >= 0 && studentId < node->keys[i] ) {
      i--;
    }
    if ( node->children[i]->numChildren == 4 ) { // if the child node at index where studentId should go  has no free slots, 
      node = split( node, i );                  // split the node
      if ( studentId > node->keys[i] ) { // if studentId greater than node keys[i]
        i++;
      }
    }
    insertNonfull( node->children[i], studentId ); // insert studentId into node's ith child (nonfull)
  }

} // insertNonfull

void insertMax( struct node* node, int studentId ) {
  int i = node->numChildren;
  node->keys[i] = studentId; // node's greatest key is studentId
  if ( node->isLeafNode == YES ) {
    node->numChildren = node->numChildren + 1;
  } else {
    if ( node->children[i]->numChildren == 4 ) { // if slotting in studentId made 4 children
      split( node, i );
      i++;
    }
    insertMax( node->children[i], studentId );
  }

} // insertMax

void insert( struct node* root, int studentId ) {
  int i = root->numChildren;
  if ( i == 4 ) { // if node has no free slots
    struct node* node = (struct node*) malloc( sizeof(struct node) ); // allocate new node
    node->isLeafNode = NO;
    node->numChildren = 1;
    node->children[0] = root;
    node->keys[0] = root->keys[3];
    println(" root->keys[3] is %d", root->keys[3] );
    root = node;
    println("trying to insert %d into node with full children. node needs to be split. ", studentId);
    split( node, 1 );
    if ( studentId > node->keys[1] ) {
      println(" studentId %d > node->keys[1] %d ", studentId, node->keys[1]);
      insertMax( node, studentId );
    } else {
      println(" studentId %d < node->keys[1] %d ", studentId, node->keys[1]);
      insertNonfull( node, studentId );
    }
  } else {
    if ( studentId > root->keys[i] ) { // if studentId > the the node's greatest child
      println(" inserting max studentId %d, greater than root->keys[i] (%d) ", studentId, root->keys[i] );
      insertMax( root, studentId ); // then studentId is the max for this node
    } else {
      println(" inserting into nonfull node studentId %d, less than root->keys[i] (%d) ", studentId, root->keys[i] );
      insertNonfull( root, studentId ); // else insert amidst the children nodes
    }
  }

}// insert

int programExit( void ) {
  return 0;
}

int main( int argc, char *argv[] ) {
   
  struct node* root = NULL;
  root = createTree(); // points to root node of tree
  struct item *courseList = NULL;
  FILE  *fp = NULL;
  char separator;

  if ( argc != 2 ) {
    exit(1);
  } else {
    if ( (fp = fopen(argv[1],"r")) == NULL ) {
      println("Unknown file");
      exit(1);
    } else {
      while ( !feof(fp) ) {
        // courseList = CreateItem( courseList );
        struct courseData *courseData = (struct courseData*) malloc(sizeof(struct courseData));
        int studentId;
        fscanf( fp, "%d %s %s %s", &studentId, courseData->courseId, courseData->courseName, courseData->grade );
        fscanf( fp, "%c", &separator );
        // search to determine if studentId is in tree or not
        // if studentId in tree (search() returned us leaf node x )
          // i = iterate keys[0,4] of x to see where we need to insert our studentId
          // insert item into i->courseList
        // else (search returned null)
          // create a node with courseList courseData
          // insert the node into the tree
          //courseList = InsertItem( node->courseList, courseData->courseId, courseData ); // append course list
        insert( root, studentId );
      }
      fclose(fp);
    }
  }

  char cmd[100];
  int execute = YES;

  while ( execute ) {
    printf("Please enter your action: ");
    scanf("%s", cmd);

    if ( equals(cmd, "quit") || equals(cmd, "exit") ) { //action IS "quit" or "exit"
      execute = NO;
    } else if ( equals(cmd, "print") ) {
      printNode( root );
    } else if ( equals(cmd, "search") || equals(cmd, "find") ) {
      search( root, 111245 );// todo - first record id
      search( root, 333245 );// todo - first record id
    }
  }

  programExit();
  exit(0);   
  
  return 0;
  
}
