#ifndef _LIST
#define _LIST

/**
 * @file list.h
 * @author Lorenzo Rossi - https://github.com/lorossi/better-lists
 * @date 26/10/2021
 * @version 1.1.0
 * @brief Simple linked list library made because I wanted to kill some time.
 * By default, the data stored inside the list is int.
 * define CHARLIST, STRINGLIST, FLOATLIST, DOUBLELIST, respectively to store chars, strings, floats and double.
 * if you want to use some custom data (struct, for example) define CUSTOMTYPE as the type you want to store.
 * Some functions (like printList) won't work.
*/

#ifdef CUSTOMTYPE
typedef CUSTOMTYPE Data;
#elif defined(CHARLIST)
typedef char Data;
#elif defined(STRINGLIST)
#define STRINGSIZE 1000
typedef char *Data[STRINGSIZE];
#elif defined(FLOATLIST)
typedef float Data;
#elif defined(DOUBLELIST)
typedef double Data;
#else
typedef int Data;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @internal
 * @brief This structure defines a base node. It contains links to previous and next node, as well as saved data.
 */
typedef struct node
{
  Data data;             /**< Data contained in node. Its type can be set by defining the constants CUSTOMTYPE, CHARLIST, STRINGLIST, FLOATLIST, DOUBLELIST before importing the header. */
  struct node *previous; /**< Pointer to previous node */
  struct node *next;     /**< Pointer to next node */
} Node;

/**
 * @internal
 * @brief This structure defines a base list. It contains pointers to first and last item, as well as its length.
 */
typedef struct list
{
  Node *head; /**< Pointer to first node */
  Node *tail; /**< Pointer to last node */
  int length; /**< Number of nodes in list */
} List;

/**
 * @internal
 * @brief This structure defines an iterator. It contains pointers to first, current and next items.
 */
typedef struct iterator
{
  struct node *previous; /**< Pointer to previous node */
  struct node *next;     /**< Pointer to next node */
  struct node *current;  /**< Pointer to current node */
  int index;
} Iterator;

List *listCreate();
void listDelete(List *list);
int listGetItem(List *list, Data *destination, int index);
int listGetFirstItem(List *list, Data *destination);
int listGetLastItem(List *list, Data *destination);
int listAddItem(List *list, Data *data, int index);
int listAppend(List *list, Data *data);
int listPrepend(List *list, Data *data);
int listReplaceItem(List *list, Data *newvalue, int index);
int listReplaceItemByValue(List *list, Data *oldvalue, Data *newvalue);
int listCountReplace(List *list, Data *oldvalue, Data *newvalue, int replace_count);
int listRemoveItem(List *list, Data *destination, int index);
int listRemoveItemByValue(List *list, Data *oldvalue);
int listCountRemove(List *list, Data *oldvalue, int remove_count);
int listPop(List *list, Data *last);
int listUnshift(List *list, Data *destination);
int listSwap(List *list, int first_index, int second_index);
int dataInList(List *list, Data *data);
int listToArray(List *list, Data *array);
void printList(List *list, char *end);
void printListReverse(List *list, char *end);
int listGetSize(List *list);
int listSort(List *list, int reverse);
void listShuffle(List *list);

// Iterator related functions
Iterator *iteratorCreate(List *list, int start);
void iteratorDelete(Iterator *it);
int iteratorEnded(Iterator *it);
int iteratorStarted(Iterator *it);
int iteratorNext(Iterator *it);
int iteratorPrevious(Iterator *it);
Data iteratorGetData(Iterator *it, Data *destination);

#endif
