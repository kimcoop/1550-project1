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

void printNode( struct node* node ) { //Print the node
  if ( node != NULL ) {
    println(">>NODE: numChildren: %d, isLeafNode?: %d", node->numChildren, node->isLeafNode);
    println(">>(printing node's keys to index %d)", node->numChildren);
    int i;
    for ( i = 0; i < node->numChildren && i < 100; i++ ) {
      println(">>keys[%d]=%d, \t", i, node->keys[i]);
    }
    if ( node->isLeafNode == YES ) { // then has courseData linkedList
      println(">>(printing node's children course data to index %d)", node->numChildren);
      int i;
      for ( i = 0; i < node->numChildren && i < 100; i++ ) {
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
  struct node* root = (struct node*) malloc( sizeof(struct node) );
  root->isLeafNode = YES;
  root->numChildren = 0;
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
    } 
  }

  y->numChildren = median;
  for ( j=node->numChildren; j >= i; j-- ) {
    node->children[j] = node->children[i];
    node->keys[j] = node->keys[i];
  }

  node->children[i+1] = z; // plop new node into place
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

struct node* insertNonfull( struct node* node, int studentId, struct item* item ) {
  println("insertNonfull");
  int i = node->numChildren;
  if ( node->isLeafNode == YES ) {
    while ( i > 0 && studentId < node->keys[i-1] ) { // while we have at least one child node and the studentId is less than current key
      node->keys[i] = node->keys[i-1]; // shift keys right by one node
      i--;
    }
    node->keys[i] = studentId; // slot studentId in the right place
    if ( node->courseList[i] != NULL ) {
      println(" node->courseList[%d] was null, now we're creating it ", i);
      // node->courseList[i] = CreateItem( item );
    }
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
    if ( node->courseList[i] != NULL ) {
      println(" node->courseList[%d] was null, now we're creating it ", i);
      // node->courseList[i] = CreateItem( item );
    }
    node->courseList[i] = InsertItem( node->courseList[i], item );
    node->numChildren = node->numChildren + 1;
  } else {
    if ( node->children[i-1]->numChildren == 4 ) { // if slotting in studentId made 4 children
      node = split( node, i );
      i++;
    }
    printNode(node->children[i-1]);
    node = insertMax( node->children[i-1], studentId, item );
  }
  return node;
} // insertMax

struct node* insertData( struct node* root, int studentId, char* courseId, char* courseName, char* grade ) { // char courseId[7], courseName[8], grade[3];

  struct item* item = CreateItemWithData( courseId, courseName, grade );
  PrintItem( item );
  root = insert( root, studentId, item );
  return root;

} // insertData

struct node* insert( struct node* root, int studentId, struct item* item ) {
// returns root node
  println(" insert. root->children = %d ", root->numChildren);
  int i = root->numChildren;
  if ( i == 4 ) { // if node has no free slots
    struct node* node = (struct node*) malloc( sizeof(struct node) ); // allocate new node
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
  println( "freeNode" );
  if ( node != NULL && node->numChildren > 0 ) {
    int i;
    for (i = 0; i < node->numChildren; i++) {
      freeNode( node->children[i] );
    }
    free( node );
  }
}

void freeTree( struct node* root ) {
  println( "freeTree ");
  printNode (root );
  if ( !root->isLeafNode && root->numChildren > 0 ) {
    int i;
    for ( i = 0; i < root->numChildren; i++ ) {
      freeNode( root->children[i] );
    }
  }
  println(" freeing root node ");
  free( root );
} // freeTree

struct node* loadFile( struct node* root, char* filename ) {
  println("loadFile");
  struct item *courseList = NULL;
  FILE  *fp = NULL;
  char separator;
  if ( (fp = fopen( filename, "r" )) == NULL ) {
    println("Unknown file");
    exit(1);
  } else {
    while ( !feof(fp) ) {
      // courseList = CreateItem( courseList );
      int studentId;
      // char *courseId, *courseName, *grade;
      char courseId[7], courseName[8], grade[3];
      fscanf( fp, "%d %s %s %s", &studentId, courseId, courseName, grade );
      fscanf( fp, "%c", &separator );
     
      println("");
      root = insertData( root, studentId, courseId, courseName, grade );
    } // end while 

    fclose(fp);
  }
  return root;

} // loadFile

int main( int argc, char *argv[] ) {
  
  struct node* root = NULL;
  root = createTree(); // points to root node of tree
  
  if ( argc != 2 ) {
    println("No filename provided." );
  } else {
    root = loadFile( root, argv[1] );
  }

  int execute = YES;

  while ( execute ) {
    char cmd[MAX_COMMAND_SIZE];
    int studentId, studentId_a, studentId_b;
    char courseId[7], courseName[8], grade[3];
    char filename[MAX_COMMAND_SIZE];

    printf("Please enter your action: ");
    scanf("%s", cmd);

    if ( strEquals(cmd, "exit") ) {
      execute = NO;

    } else if ( strEquals(cmd, "printtree") ) {
      printTree( root );

    } else if ( strEquals(cmd, "printroot") || strEquals(cmd, "print") ) {
      printNode( root );
    
    } else if ( strEquals(cmd, "find") ) {
      scanf("%d", &studentId);
      search( root, studentId );
    
    } else if ( strEquals(cmd, "ins") ) { // studentId, courseId, courseName, grade
      scanf("%d %s %s %s", &studentId, courseId, courseName, grade);
      println("inserting %d %s %s %s", studentId, courseId, courseName, grade);
      root = insertData( root, studentId, courseId, courseName, grade); // insert new data
    
    } else if ( strEquals(cmd, "load") ) { // filename
      scanf("%s", filename);
      println("load from file %s", filename);
      loadFile( root, filename );
    
    } else if ( strEquals(cmd, "range") ) { // studentId_a, studentId_b
      scanf("%d %d", &studentId_a, &studentId_b);
      println("range");
    
    } else if ( strEquals(cmd, "gpa") ) { // gpa <studentId> or gpa<studentId_a> <studentId_b> (range) 
      scanf("%d", &studentId); // for now, assume we have one GPA input
      println("finding GPA for studentId %d", studentId);
    
    } else if ( strEquals(cmd, "verify") ) { // check all nodes to ensure properties of 2-4 tree
      println("verify");

    } else if ( strEquals(cmd, "clearbuffer") || strEquals(cmd, "clearbuf") ) {
      println("\n");
    }

    strlcpy( cmd, "", sizeof( cmd ) );

  } // end while

  freeTree( root );
  exit(0);  
 
  return 0;
 
}
