#include	<stdio.h>

#define		MAXSIZE	64
#define		MAX_COMMAND_SIZE 200

#define   YES 1
#define   NO  0

#define println(...) printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n")
#define strEqual(a, b) !strcmp(a, b)

struct item {
  struct item *next;
  char courseId[7], courseName[8], grade[3];
};

struct courseMap {
  char courseId[7];
  int enrollments;
};

struct node {
  int numChildren; // number of keys currently stored
  int keys[4]; // the n keys themselves
  int isLeafNode; // true if node is a leaf node
  struct node* children[4]; // array with four pointers to node structs
  struct item* courseList[4]; // array with four pointers to first courseData in linkedlist for each child node
};

void PrintItem(struct item *p);
struct node* insert( struct node* root, int studentId, struct item* item );

struct node* search( struct node* node, int studentId );

int calculateGPA( struct node* root, int studentId );
int gradepointForGrade( char* grade );