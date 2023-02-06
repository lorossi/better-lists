#ifndef _LIST
#define _LIST

/**
 * @file list.h
 * @author Lorenzo Rossi - https://github.com/lorossi/better-lists
 * @date 26/10/2021
 * @version 1.1.0
 * @brief Simple linked list library made because I wanted to kill some time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

union Data
{
  int i;
  char c;
  float f;
  double d;
  char *s;
  void *p;
};

typedef enum
{
  INTEGER,
  CHAR,
  FLOAT,
  DOUBLE,
  STRING,
  POINTER,
} list_type;

/**
 * @internal
 * @brief This structure defines a base node. It contains links to previous and next node, as well as saved data.
 */
typedef struct node
{
  union Data data;       /**< Data contained in node. */
  struct node *previous; /**< Pointer to previous node */
  struct node *next;     /**< Pointer to next node */
} Node;

/**
 * @internal
 * @brief This structure defines a base list. It contains pointers to first and last item, as well as its length.
 */
typedef struct list
{
  Node *head;     /**< Pointer to first node */
  Node *tail;     /**< Pointer to last node */
  int length;     /**< Number of nodes in list */
  list_type type; /**< Type of data contained in node. */
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

List *listCreate(list_type type);
void listDelete(List *list);
int listGetItem(List *list, union Data *destination, int index);
int listGetFirstItem(List *list, union Data *destination);
int listGetLastItem(List *list, union Data *destination);
int listAddItem(List *list, union Data *destination, int index);
int listPush(List *list, union Data *data);
int listPrepend(List *list, union Data *data);
int listReplaceItem(List *list, union Data *new_value, int index);
int listReplaceItemByValue(List *list, union Data *old_value, union Data *new_value);
int listCountReplace(List *list, union Data *old_value, union Data *new_value, int replace_count);
int listCountItem(List *list, union Data *value);
int listRemoveItem(List *list, union Data *data, int index);
int listRemoveItemByValue(List *list, union Data *old_value);
int listCountRemove(List *list, union Data *old_value, int remove_count);
int listPop(List *list, union Data *last);
int listShift(List *list, union Data *destination);
int listSwap(List *list, int first_index, int second_index);
int dataInList(List *list, union Data *destination);
int listToArray(List *list, union Data *array);
int listGetSize(List *list);
int listSort(List *list, int reverse); // TODO create function to sort list using a comparator
void listShuffle(List *list);

// Iterator related functions
Iterator *iteratorCreate(List *list, int start);
void iteratorDelete(Iterator *it);
int iteratorEnded(Iterator *it);
int iteratorStarted(Iterator *it);
int iteratorNext(Iterator *it);
int iteratorPrevious(Iterator *it);
int iteratorGetData(Iterator *it, union Data *destination);
int iteratorSetData(Iterator *it, union Data *new_value);
int iteratorGetIndex(Iterator *it);

#endif
