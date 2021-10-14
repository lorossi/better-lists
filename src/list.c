#include "list.h"

/* Pivate function. Create a node with link to previous and next node. */
Node *_createNode(Node *previous, Node *next, Data *data)
{
  Node *new = NULL;
  new = (Node *)malloc(sizeof(Node));

  if (previous != NULL)
    new->previous = previous;
  else
    new->previous = NULL;

  if (next != NULL)
    new->next = next;
  else
    new->next = NULL;

  new->data = *data;

  return new;
}

/* Private function. Find node in a list by its index. */
Node *_findNodeByIndex(List *list, int index)
{
  if (index >= listGetLength(list))
    return NULL;

  Node *current;
  int count, increment;

  // if the node is closer to then end than to the start, go backwards
  if (index > listGetLength(list) / 2)
  {
    current = list->tail;
    count = listGetLength(list) - 1;
    increment = -1;
  }
  else
  {
    current = list->head;
    count = 0;
    increment = 1;
  }

  while (current != NULL)
  {
    if (index == count)
      return current;

    count += increment;

    if (increment == 1)
      current = current->next;
    else
      current = current->previous;
  }

  return NULL;
}

/* Private function. Find node in a list by its value. */
Node *_findNodeByValue(List *list, Data *data)
{
  Node *current = list->head;

  while (current != NULL)
  {
    if (current->data == *data)
      return current;

    current = current->next;
  }

  return NULL;
}

/* Private function. Delete a a node. */
void _deleteNode(Node *node)
{
  free(node);
  return;
}

/* Private function. Print the value of the node. */
void _printNode(Node *node, int index)
{
#ifdef CUSTOMLIST
  printf("item %i: non printable", index);
#elif defined(CHARLIST)
  printf("item %i: %c\n", index, node->data);
#elif defined(STRINGLIST)
  printf("item %i: %s\n", index, node->data);
#elif defined(FLOATLIST)
  printf("item %i: %f\n", index, node->data);
#elif defined(DOUBLELIST)
  printf("item %i: %f\n", index, node->data);
#else
  printf("item %i: %i\n", index, node->data);
#endif
}

/* Get the data in a node. Returns -1 if error or the size of the data if successful.
If destination is NULL, no value is passed. */
int nodeGetData(Node *node, Data *destination)
{
  if (node != NULL)
  {
    if (destination != NULL)
      *destination = node->data;

    return sizeof(*destination);
  }

  return -1;
}

/* Set the data in a node. Returns -1 if error or the size of the data if successful. */
int nodeSetData(Node *node, Data *data)
{
  if (node != NULL)
  {
    node->data = *data;
    return sizeof(*data);
  }

  return -1;
}

/* Get the following node. */
Node *nodeGetNext(Node *node)
{
  return node->next;
}

/* Get the previous node. */
Node *nodeGetPrevious(Node *node)
{
  return node->previous;
}

/* Create and return a list. */
List *listCreate()
{
  List *l = malloc(sizeof(List));
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
  return l;
}

/* Delete a list and all of its nodes. */
void listDelete(List *list)
{
  Node *current = list->head;

  while (current != NULL)
  {
    _deleteNode(current);
    current = current->next;
  }

  free(list);

  return;
}

/* Get an item in a list by its index. Returns -1 if error or the size of the data if successful. */
int listGetItem(List *list, Data *destination, int index)
{
  Node *node = _findNodeByIndex(list, index);
  if (node == NULL)
    return -1;

  if (nodeGetData(node, destination))
    return sizeof(*destination);

  return -1;
}

/* Get the first item in a list. Returns -1 if error or the size of the data if successful.
If destination is NULL, no value is passed. */
int listGetFirstItem(List *list, Data *destination)
{
  if (list->head != NULL)
  {
    if (destination != NULL)
      *destination = list->head->data;

    return sizeof(*destination);
  }

  return -1;
}

/* Get the last item in a list. Returns -1 if error or the size of the data if successful. 
If destination is NULL, no value is passed. */
int listGetLastItem(List *list, Data *destination)
{
  if (list->tail != NULL)
  {
    if (destination != NULL)
      *destination = list->tail->data;

    return sizeof(*destination);
  }

  return -1;
}

/* Append an item to the list. */
int listAppend(List *list, Data *data)
{
  list->length++;

  if (list->head == NULL)
  {
    // no head, create one
    Node *head = _createNode(NULL, NULL, data);
    // update list
    list->head = head;

    return 0;
  }

  if (list->tail == NULL)
  {
    // no tail, create one
    // there's only one item in the list, the head
    Node *tail = _createNode(list->head, NULL, data);
    // update list and tail
    list->head->next = tail;
    list->tail = tail;

    return 1;
  }

  // head and tail exist, change tail
  // update the formerly last node
  Node *new_tail = _createNode(list->tail, NULL, data);
  list->tail->next = new_tail;
  list->tail = new_tail;

  return list->length - 1;
}

/* Add an item in front of a list. Returns position. */
int listPrepend(List *list, Data *data)
{
  list->length++;

  if (list->head == NULL)
  {
    // no head, create one
    Node *head = _createNode(NULL, NULL, data);
    // update list
    list->head = head;

    return 0;
  }

  if (list->tail == NULL)
  {
    // no tail, create one
    // there's only one item in the list, the head
    // update list
    list->tail = list->head;
    // create new head
    Node *head = _createNode(NULL, list->tail, data);

    list->tail->previous = head;
    // set new head
    list->head = head;
    return 0;
  }

  // head and tail exist, change head
  // update the formerly first node
  Node *new_head = _createNode(NULL, list->head, data);
  list->head->previous = new_head;
  list->head = new_head;

  return 0;
}

/* Remove an item from the list according to its value. Returns -1 if error or the size of the data if successful.
If destination is NULL, no value is passed. */
int listRemoveItem(List *list, Data *destination, int index)
{
  Node *current = _findNodeByIndex(list, index);

  if (current == NULL)
    return -1;

  // update previous
  if (index != 0)
    current->previous->next = current->next;
  else
    list->head = current->next;

  // update next
  if (index != list->length - 1)
    current->next->previous = current->previous;
  else
    list->tail = current->previous;

  if (destination != NULL)
    *destination = current->data;

  _deleteNode(current);
  list->length--;

  return sizeof(*destination);
}

/* Pop the list, removing its last item. Returns -1 if error or the size of the data if successful. */
int listPop(List *list, Data *last)
{
  if (listRemoveItem(list, last, list->length - 1) != -1)
    return sizeof(*last);

  return -1;
}

/* Unshift the list, removing its first item. Returns -1 if error or the size of the data if successful. */
int listUnshift(List *list, Data *first)
{
  if (listRemoveItem(list, first, 0) != -1)
    return sizeof(*first);

  return -1;
}

/* Remove all occurrences of a certain value from the list. Return number of removed items. If remove count is -1,
removes all occurencies. */
int listFindAndRemoveItems(List *list, Data *data, int remove_count)
{
  int count = 0;

  while (count < remove_count || remove_count == -1)
  {
    Node *current = _findNodeByValue(list, data);

    if (current == NULL)
      break;

    if (current->previous != NULL && current->next != NULL)
    {
      // not first or last node
      current->previous->next = current->next;
      current->next->previous = current->previous;
      _deleteNode(current);
    }
    else if (current->previous == NULL && current->next == NULL)
    {
      // removing the only item
      _deleteNode(list->head);
      list->head = NULL;
      list->tail = NULL;
    }
    else if (current->previous == NULL)
    {
      // deleting the first node in the list
      list->head = current->next;
      list->head->previous = NULL;
    }
    else if (current->next == NULL)
    {
      // deleting the last node in the list
      list->tail = current->previous;
      list->tail->next = NULL;
    }

    count++;
    list->length--;
  }

  return count;
}

/* Add item to the list. Return its position. */
int listAddItem(List *list, Data *data, int position)
{
  if (position > list->length - 1)
  {
    // add tail
    listAppend(list, data);

    return list->length - 2;
  }

  if (position == 0)
  {
    listPrepend(list, data);
    return 0;
  }

  Node *previous = _findNodeByIndex(list, position - 1);
  Node *next = _findNodeByIndex(list, position);
  Node *new = _createNode(previous, next, data);
  previous->next = new;
  next->previous = new;
  list->length++;

  return position;
}

/* Check if data is in list. If found, returns its size. If not found, returns -1 */
int dataInList(List *list, Data *data)
{
  Node *node;
  node = _findNodeByValue(list, data);

  if (node != NULL)
    return sizeof(node);

  return -1;
}

/* Make an array out of the list. */
void listToArray(List *list, Data *array)
{
  for (int i = 0; i < listGetLength(list); i++)
  {
    Node *node = _findNodeByIndex(list, i);
    nodeGetData(node, &array[i]);
  }
}

/* Print the list in a readable manner */
void printList(List *list, char *end)
{

  Node *current = list->head;
  int index = 0;

  while (current != NULL)
  {
    _printNode(current, index);

    current = current->next;
    index++;
  }

  printf("%s", end);
}

/* Print the list in a readable manner, in reverse */
void printListReverse(List *list, char *end)
{
  Node *current;

  if (list->tail != NULL)
    current = list->tail;
  else
    current = list->head;

  int index = list->length - 1;

  while (current != NULL)
  {
    _printNode(current, index);

    current = current->previous;
    index--;
  }

  printf("%s", end);
}

/* Return the length of the list */
int listGetLength(List *list)
{
  return list->length;
}