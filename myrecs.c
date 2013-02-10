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
  println("------node------");
  println("numChildren: %d", node->numChildren);
  println("isLeafNode?: %d", node->isLeafNode);
  if ( node->isLeafNode > 2 ) {  // TODO this is icky WHYYY
    println(" WRONG ");
   }
  if ( node->numChildren > 0 ) {
    int i;
    for ( i=0; i<node->numChildren; i++ ) {
      printf( "keys[%d]=%d\t", i, node->keys[i]  );
    }
    println("");
  }
  if ( node->isLeafNode ) { // then has courseData linkedList
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
  println(" in search, node key[0] is %d  ", node->keys[0]);
  int i = 1;

  while ( i < node->numChildren && studentId > node->keys[i] ) { // 
    println(" in search 1"); // studentId greater than child at i
    i++;
  }

  if ( i <= node->numChildren && studentId == node->keys[i] ) { // studentId exactly matches node's child at i
    println(" in search 2");
    return node; // return (node, i)
  } else if ( node->isLeafNode  ) { // if we hit a leaf node, we've run out
    println(" in search 3");
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
    for ( j=1; j>=t; j-- ) {
      z->keys[j] = y->keys[j+t];
    }

    if ( y->isLeafNode == NO ) {
      for ( j=1; j>= t; j-- ) {
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
  if ( node->isLeafNode ) {
    while ( i >= 1 && studentId < node->keys[i] ) { // while we have at least one child node and the studentId is less than current key
      node->keys[i+1] = node->keys[i]; // shift keys left by one node
      i--;
    }
    node->keys[i+1] = studentId; // slot studentId in the right place
    node->numChildren = node->numChildren + 1;
  } else {
    while ( i >= 1 && studentId < node->keys[i] ) {
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
  node->keys[i+1] = studentId; // node's greatest key is studentId
  if ( node->isLeafNode ) {
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
    node->children[1] = root;
    node->keys[1] = root->keys[3];
    root = node;
    println(" node needs to be split ");
    split( node, 1 );
    if ( studentId > node->keys[2] ) {
      insertMax( node, studentId );
    } else {
      insertNonfull( node, studentId );
    }
  } else {
    if ( studentId > root->keys[i] ) { // if studentId > the the node's greatest child
      println("3: %d", root->isLeafNode);
      println(" inserting max studentId %d ", studentId );
      insertMax( root, studentId ); // then studentId is the max for this node
    } else {
      println("2: %d", root->isLeafNode);
      println(" inserting into nonfull node studentId %d ", studentId );
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
        insert( root, studentId );
        // search to determine if studentId is in tree or not
        // if studentId in tree (search() returned us leaf node x )
          // i = iterate keys[0,4] of x to see where we need to insert our studentId
          // insert item into i->courseList
        // else (search returned null)
          // create a node with courseList courseData
          // insert the node into the tree
          //courseList = InsertItem( node->courseList, courseData->courseId, courseData ); // append course list
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
      
      int studentIdForSearch = 111245 ; // todo - first record id
      struct node* root = search( root, studentIdForSearch );
    }
  }

  programExit();
  exit(0);   
  
  return 0;
  
}