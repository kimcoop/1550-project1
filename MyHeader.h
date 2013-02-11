#include	<stdio.h>

// #define	 	YES	1
// #define		NO	2
#define		FALSE	20
#define		TRUE	30
#define		MAXSIZE	64
#define		MAX_COMMAND_SIZE 200

#define   YES 1
#define   NO  0

#define println(...) printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n")
#define strEquals(a, b) !strcmp(a, b)

struct courseData {
  char courseId[7], courseName[8], grade[3];
  struct courseData *next;
};

struct node {
  int numChildren; // number of keys currently stored
  int keys[4];// the n keys themselves
  int isLeafNode; // true if node is a leaf node
  struct node* children[4]; // array with four elements, each of type struct node
  struct item* courseList; // pointer to first courseData in linkedlist
};

void PrintItem(struct item *p);