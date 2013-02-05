/*
Kim Cooperrider
kac162
cs1550
Project 1
*/

#define   YES 1
#define   NO  0
#define   FALSE 20
#define   TRUE  30
#define   MAXSIZE 64

#define println(...) printf( __VA_ARGS__ ); printf("\n")
#define equals(a, b) !strcmp(a, b)

#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.c"
#include "OrderedLinkedList.c"

typedef struct { //format: 111245 PH0201 RESPHYS F
  char studentId[7], courseId[7], courseName[8], grade[3];
} Record;

struct node {
  int numChildren; // number of keys currently stored
  int keys[4];// the n keys themselves
  int isLeafNode; // true if node is a leaf node
  struct node* children[4]; // array with four elements, each of type struct node
};

void printRecord( Record* record ) { //Print the record
  println("------record------");
  println("studentId:\t%s", record->studentId);
  println("courseId:\t%s", record->courseId);
  println("courseName:\t%s", record->courseName);
  println("grade:\t%s", record->grade);
} // printRecord

void printNode( struct node* node ) { //Print the node
  println("------node------");
  println("num children:\t\t%d", node->numChildren);
  println("leaf?:\t\t%d", node->isLeafNode);
} // printNode

struct node* init( void ) { // return a pointer to the root node
  println("I'm in the init function.");

  struct node* root = (struct node*) malloc( sizeof(struct node) );
  root->isLeafNode = YES;
  root->numChildren = 0;
  return root;
} // init

struct node* search( struct node* node, int k ) {

  println("I'm in the search function.");
  int i = 1;

  while ( i < node->numChildren && k > node->keys[i] ) {
    i++;
    println("in search. i is %d", i);
  }

  if ( i <= node->numChildren && k == node->keys[i] ) {
    println("returning");
    return node; // return (node, i)
  }

  if ( node->isLeafNode == YES ) {
    println("node is leaf node. returning nil"); // no node found
    return NULL;
  } else {
    puts("recursing");
    return search( node->children[i], k ); // return
  }

} // search

struct node* split( struct node* node, int i  ) {

  struct node* z = (struct node*) malloc( sizeof(struct node) );

  if ( node->numChildren > 0 ) {
    struct node* y = (struct node*) node->children[i]; 
    printNode(y);
    int t = y->numChildren / 2;
    z->numChildren = y->numChildren - t;
    println(" z_numChildren is %d", z->numChildren);
    z->isLeafNode = y->isLeafNode;

    int j;
    for ( j=1; j>=t; j-- ) {
      z->keys[j] = y->keys[j+t];
    }

    if ( !y->isLeafNode ) { //meh?
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
    printNode( node );

  } else {
    println(" node has no children ");
  }

  return z;

} // split

void insertNonfull( struct node* node, int k ) {

  println("insertNonfull");
  int i = node->numChildren;
  if ( node->isLeafNode ) {
    while ( i >= 1 && k < node->keys[i] ) {
      node->keys[i+1] = node->keys[i];
      i--;
    }
    node->keys[i+1] = k;
    node->numChildren = node->numChildren + 1;
  } else {
    while ( i >= 1 && node->keys[i] ) {
      i--;
    }
    if ( node->children[i]->numChildren == 4 ) {
      split( node, i );
      if ( k > node->keys[i] ) {
        i++;
      }
    }
    insertNonfull( node->children[i], k );
  }

} // insertNonfull

void insertMax( struct node* node, int k ) {
  println("I'm in insertMax.");

  int i = node->numChildren;
  if ( node->isLeafNode ) {
    node->keys[i+1] = k;
    node->numChildren = node->numChildren + 1;
  } else {
    node->keys[i+1] = k;
    if ( node->children[i]->numChildren == 4 ) {
      split( node, i );
    }
    insertMax( node->children[i], k );
  }

} // insertMax

void insert( struct node* root, int k ) {
  println("I'm in the insert function.");

  int i = root->numChildren;
  if ( i == 4 ) {
    struct node* node = (struct node*) malloc( sizeof(struct node) );
    node->isLeafNode = NO;
    node->numChildren = 1;
    node->children[1] = root;
    node->keys[1] = root->keys[4];
    root=node;
    split( node, 1 );
    if ( k > node->keys[2] ) {
      insertMax( node, k );
    } else {
      insertNonfull( node, k );
    }
  } else {
    if ( k > root->keys[i] ) {
      println("k (%d) is > root->keys[i] (%d)", k, root->keys[i]);
      insertMax( root, k ); //meh?
    } else {
      println("k (%d) is < root->keys[i] (%d)", k, root->keys[i]);
      insertNonfull( root, k );
    }
  }

}// insert

int programExit( void ) {
  println("I'm in the exit function.");
  return 0;
}

int main( int argc, char *argv[] ) {
   
  struct node* root = init();
  struct item *List = NULL;
  char  myword[MAXSIZE], buffer[MAXSIZE];
  FILE  *fp=NULL;

  if ( argc != 2 ) {
    println("expected syntax: myexe inputfile");
    exit(1);
  } else {
    if ( (fp = fopen(argv[1],"r")) == NULL ) {
      println("Unknown file");
      exit(1);
    } else {
      List = CreateItem( List );
      while ( !feof(fp) ) {
        Record* newRecord = (Record *) malloc(sizeof(Record));
        println("%s", myword);
        fscanf( fp, "%s %s %s %s", newRecord->studentId, newRecord->courseId, newRecord->courseName, newRecord->grade );
        // printRecord( newRecord );
        List = OrderedInsertItem( List, myword );
      }
      fclose(fp);   
      OrderedPrintItem(List);
    }
  }
  /* delete the smallest element in the list */
  // List = OrderedDeleteItem(List, GetSmallestItem(List));
  // println("Have deleted the Smallest element of the List");
  // OrderedPrintItem(List);

  char command[100];
  int execute = YES;

  while ( execute == YES ) {
   
    printf("Please enter your action: ");
    scanf("%s", command);

    if ( equals(command, "quit") || equals(command, "exit") ) { //action IS "quit" or "exit"
      execute = NO;
    } else {
      execute = YES;
    }
     
    println("you entered %s", command);

  }

  programExit();
  exit(0);   
  
  return 0;
  
}
