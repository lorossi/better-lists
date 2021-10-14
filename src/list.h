/* 
  Lorenzo Rossi - 2021
  Simple linked list library made because I wanted to kill some time.
  By default, the data stored inside the list is int.
  define CHARLIST, STRINGLIST, FLOATLIST, DOUBLELIST, respectively to store chars, strings, floats and double.
  if you want to use some custom data (struct, for example) define CUSTOMTYPE as the type you want to store.
  Some functions (like printList) won't work.
*/

#ifndef _LIST
#define _LIST

#ifdef CUSTOMTYPE
typedef CUSTOMTYPE Data;
#elif defined(CHARLIST)
typedef char Data;
#elif defined(STRINGLIST)
typedef char *Data;
#elif defined(FLOATLIST)
typedef float Data;
#elif defined(DOUBLELIST)
typedef double Data;
#else
typedef int Data;
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
  Data data;
  struct node *previous;
  struct node *next;
} Node;

typedef struct
{
  Node *head;
  Node *tail;
  int length;
} List;

// Node related functions
int nodeGetData(Node *node, Data *destination);
int nodeSetData(Node *node, Data *data);
Node *nodeGetNext(Node *node);
Node *nodeGetPrevious(Node *node);

// List related functions
List *listCreate();
void listDelete(List *list);
int listGetItem(List *list, Data *data, int index);
int listGetFirstItem(List *list, Data *destination);
int listGetLastItem(List *list, Data *destination);
int listAppend(List *list, Data *data);
int listPrepend(List *list, Data *data);
int listRemoveItem(List *list, Data *destination, int index);
int listFindAndRemoveItems(List *list, Data *data, int remove_count);
int listPop(List *list, Data *last);
int listUnshift(List *list, Data *destination);
int listAddItem(List *list, Data *data, int position);
void listToArray(List *list, Data *array);
void printList(List *list, char *end);
void printListReverse(List *list, char *end);
int listGetLength(List *list);

#endif

/* 
  FUNCTIONS TO ADD:
    - data in list
    - list get node
    - list replace value
      - at index
      - from value

  TEST TO DO:
    - node get data
    - node set data
    - node get next
    - node get previous
    - what happens if I pass an empty pointer as destination? DONE
    - list to array

  GENERAL
    - check comments uniformity
*/