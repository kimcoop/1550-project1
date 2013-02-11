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

void printNode( struct node* node ) { //Print the node
  if ( node != NULL ) {
    printf(">>NODE: numChildren: %d, isLeafNode?: %d.", node->numChildren, node->isLeafNode);
    if ( node->numChildren > 0 ) {
      int i;
      printf("\t");
      for ( i = 0; i < node->numChildren && i < 100; i++ ) {
        printf("keys[%d]=%d, \t", i, node->keys[i]);
      }
    }
    if ( node->isLeafNode == YES ) { // then has courseData linkedList
      PrintItem( node->courseList );
      println("");
    }
    println("");
  } else {
    println(" NODE NULL ***" );
  }
} // printNode

void printTree( struct node* root ) {
  printNode( root );
  if ( !root->isLeafNode ) {
    println(" children ");
    int i;
    for ( i = 0; i < root->numChildren; i++ ) {
      println( "i is %d", i );
      printNode( root->children[i] );
    }
  }
} // printTree

struct node* createTree( void ) { // return a pointer to the root node
  struct node* root = (struct node*) malloc( sizeof(struct node) );
  root->isLeafNode = YES;
  root->numChildren = 0;
  root->courseList = NULL;
  return root;
} // init

struct node* search( struct node* node, int studentId ) {
  int i = 0;
  println("node->numChildren %d && studentId > node->keys[0] %d", node->numChildren, node->keys[i]);
  while ( i < node->numChildren && studentId > node->keys[i] ) {
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
    return search( node->children[i], studentId ); // return
  }

} // search

struct node* split( struct node* node, int i  ) {

  struct node* z = (struct node*) malloc( sizeof(struct node) );
  println("splitting node at position %d", i);

  println(" node->numChildren = %d", node->numChildren);
  struct node* y = (struct node*) node->children[i]; // y is node's leftmost child
  int median = y->numChildren / 2; // median is half the number of y's children
  z->numChildren = y->numChildren - median; // equally divide y's children (take median number)
  z->isLeafNode = y->isLeafNode;
  println("node passed into split: ");
  printNode( node );
  println("z (node we're constructing):");
  printNode( z );
  println("y (node's child at %d ):", i);
  printNode( y );

  int j;
  for ( j=0; j < median; j++ ) {
    println( "j=%d. median i=%d. z->keys[%d] was %d, now is %d", j, median, j, z->keys[j], y->keys[j+median]);
    z->keys[j] = y->keys[j+median];
  }

  println(" y->isLeafNode? %d", y->isLeafNode);

  if ( !y->isLeafNode ) {
    for ( j=0; j < median; j++ ) {
      z->children[j] = y->children[j+median];
    } 
  }

  y->numChildren = median;
  println("y->numChildren = %d", y->numChildren);

  for ( j=node->numChildren; j >= i; j-- ) {
    node->children[j] = node->children[i];
    node->keys[j] = node->keys[i];
  }

  println(" making node's %dth child = z", i+1 );
  node->children[i+1] = z; // plop new node into place
  println("node->keys[%d] previously=%d, now=%d ", i, node->keys[i], y->keys[median-1] );
  node->keys[i] = y->keys[median-1];
  node->numChildren = node->numChildren+1;
  println("ROOT:");
  printNode(node);
  println("Y NODE:");
  printNode(y);
  println("Z NODE:");
  printNode(z);

  return node;

} // split

struct node* insertNonfull( struct node* node, int studentId ) {
  println("insertNonfull");
  int i = node->numChildren;
  if ( node->isLeafNode == YES ) {
    while ( i > 0 && studentId < node->keys[i-1] ) { // while we have at least one child node and the studentId is less than current key
      println(" i (%d) > 0 && studentId < node->keys[%d] (%d)", i, i-1, node->keys[i-1]);
      node->keys[i] = node->keys[i-1]; // shift keys right by one node
      i--;
    }
    node->keys[i] = studentId; // slot studentId in the right place
    println("node->keys[%d] is now studentId %d ", i, studentId);
    node->numChildren = node->numChildren + 1;
    printNode(node);
  } else {
    while ( i >= 0 && studentId < node->keys[i-1] ) { // while studentId < node's greatest key
      println("studentId < node's greatest key in pos %d = %d ", i-1, node->keys[i-1] );
      i--;
    }
    if ( node->children[i]->numChildren == 4 ) { // if the child node at index where studentId should go has no free slots,
      node = split( node, i );                  // split the node
      if ( studentId > node->keys[i] ) { // if studentId greater than node keys[i]
        println("studentId %d> node->keys[i] %d", studentId, node->keys[i]);
        i++;
      }
    }
    println("about to insert studentId %d into nonfull node with greatest key %d", studentId, node->keys[i]);
    printNode( node->children[i] );
    node = insertNonfull( node->children[i], studentId ); // insert studentId into node's ith child (nonfull) // TODO - why is this i+1??
  }
  return node;
} // insertNonfull

struct node* insertMax( struct node* node, int studentId ) {
  println(" insertMax ");
  int i = node->numChildren;
  printNode( node);
  println("node->keys[%d] was previously %d, now is %d", i, node->keys[i], studentId );
  node->keys[i] = studentId; // node's greatest key is studentId
  if ( node->isLeafNode == YES ) {
    println(" node isLeafNode: ");
    node->numChildren = node->numChildren + 1;
    printNode ( node);
  } else {
    println(" node->children[%d]: ", i-1);
    printNode ( node->children[i-1]);
    if ( node->children[i-1]->numChildren == 4 ) { // if slotting in studentId made 4 children
      node = split( node, i );
      i++;
    }
    node = insertMax( node->children[i-1], studentId );
  }
  return node;
} // insertMax

struct node* insert( struct node* root, int studentId ) {
// returns root node
  int i = root->numChildren;
  if ( i == 4 ) { // if node has no free slots
    struct node* node = (struct node*) malloc( sizeof(struct node) ); // allocate new node
    node->isLeafNode = NO;
    node->numChildren = 1;
    node->children[0] = root; // new node's first child is root
    node->keys[0] = root->keys[3]; // first child of new node is greatest child of root
    root = node;
    println(" new root node: ");
    printNode(root );
    println("trying to insert %d into node with full children. node needs to be split. ", studentId);
    node = split( node, 0 );
    if ( studentId > node->keys[1] ) {
      println(" studentId %d > node->keys[1] %d ", studentId, node->keys[1]);
      node = insertMax( node, studentId );
    } else {
      println(" studentId %d < node->keys[1] %d ", studentId, node->keys[1]);
      node = insertNonfull( node, studentId );
    }
  } else {
    if ( studentId > ( i != 0 ? root->keys[i-1] : root->keys[0] ) ) { // if studentId > the the node's greatest child
      println(" inserting max studentId %d, greater than root->keys[%d]=%d ", studentId, i-1, root->keys[i-1] );
      root = insertMax( root, studentId ); // then studentId is the max for this node
    } else {
      println(" inserting into nonfull node studentId %d, less than root->keys[%d]=%d ", studentId, i-1, root->keys[i-1] );
      root = insertNonfull( root, studentId ); // else insert amidst the children nodes
    }
    return root;
  }

}// insert

void freeNode( struct node* node ) {
  if (node->numChildren > 0) {
    int i;
    for (i = 0; i < node->numChildren; i++) {
      freeNode( node->children[i] );
    }
  }
  free( node );
}

void freeTree( struct node* root ) {
  println( "freeTree ");
  println( "root->numChildren = %d ", root->numChildren);
  if (root->numChildren > 0) {
    int i;
    for (i = 0; i < root->numChildren; i++) {
      freeNode( root->children[i] );
    }
  }
  free( root );
} // freeTree

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
        root = insert( root, studentId );
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
      // printTree( root );
      printNode( root );
    } else if ( equals(cmd, "search") || equals(cmd, "find") ) {
      search( root, 111245 );
      // search( root, 333245 );
      // search( root, 111266 );
    }
  }

  freeTree( root );
  exit(0);  
 
  return 0;
 
}
