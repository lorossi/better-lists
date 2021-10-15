#include "list.h"

/* Private function. Create a node with link to previous and next node. */
Node *_nodeCreate(Node *previous, Node *next, Data *data)
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

/* Private function. Delete a a node. */
void _nodeDelete(Node *node)
{
  free(node);
  return;
}

/* Private function. Find node in a list by its index. */
Node *_findNodeByIndex(List *list, int index)
{
  if (index >= listGetSize(list))
    return NULL;

  Node *current;
  int count, increment;

  // if the node is closer to then end than to the start, go backwards
  if (index > listGetSize(list) / 2)
  {
    current = list->tail;
    count = listGetSize(list) - 1;
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

/* Private function. Find node index in a list by its value. */
int _findNodeIndexByValue(List *list, Data *data)
{
  Node *current = list->head;
  int index = 0;

  while (current != NULL)
  {
    if (current->data == *data)
      return index;

    current = current->next;
    index++;
  }

  return -1;
}

/* Private function. Print the value of the node. */
void _nodePrint(Node *node, int index)
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

/* Private function. Get the data in an item. Returns -1 if an error is encountered or the size of the data in case of success.
If destination is NULL, no values is read. */
int _nodeGetData(Node *node, Data *destination)
{
  if (node != NULL)
  {
    if (destination != NULL)
    {
      *destination = node->data;
      return sizeof(*destination);
    }
  }

  return -1;
}

/* Private function. Set the data in an item. Returns -1 if an error is encountered or the size of the data in case of success. */
int _nodeSetData(Node *node, Data *data)
{
  if (node != NULL)
  {
    node->data = *data;
    return sizeof(*data);
  }

  return -1;
}

/* Private function. Retrieve a node from a list by its index. Returns -1 if an error is encountered or the size of the data in case of success. */
int _listGetNode(List *list, Node *destination, int index)
{
  Node *node;
  node = _findNodeByIndex(list, index);

  if (node != NULL)
  {
    destination = node;
    return sizeof(*node);
  }

  return -1;
}

/* Private function. Retrieve a node from a list by its value. Returns -1 if an error is encountered or the size of the data in case of success. */
int _listGetNodeByValue(List *list, Node *destination, Data *value)
{
  Node *node;
  node = _findNodeByValue(list, value);

  if (node != NULL)
  {
    destination = node;
    return sizeof(*node);
  }

  return -1;
}

/* Create and return a list. */
List *listCreate()
{
  List *list = malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

/* Delete a list and all of its nodes. */
void listDelete(List *list)
{
  Node *current = list->head;

  while (current != NULL)
  {
    Node *next = current->next;
    _nodeDelete(current);
    current = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  free(list);

  return;
}

/* Retrieve an item in a list by its index. Returns -1 if an error is encountered or the size of the data in case of success. */
int listGetItem(List *list, Data *destination, int index)
{
  Node *node = _findNodeByIndex(list, index);
  if (node == NULL)
    return -1;

  if (_nodeGetData(node, destination))
    return sizeof(*destination);

  return -1;
}

/* Get the first item in a list. Returns -1 if an error is encountered or the size of the data in case of success.
If parameter destination is NULL, nothing is loaded. */
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

/* Get the last item in a list. Returns -1 if an error is encountered or the size of the data in case of success.
If parameter destination is NULL, nothing is loaded. */
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
    Node *head = _nodeCreate(NULL, NULL, data);
    // update list
    list->head = head;

    return 0;
  }

  if (list->tail == NULL)
  {
    // no tail, create one
    // there's only one item in the list, the head
    Node *tail = _nodeCreate(list->head, NULL, data);
    // update list and tail
    list->head->next = tail;
    list->tail = tail;

    return 1;
  }

  // head and tail exist, change tail
  // update the formerly last node
  Node *new_tail = _nodeCreate(list->tail, NULL, data);
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
    Node *head = _nodeCreate(NULL, NULL, data);
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
    Node *head = _nodeCreate(NULL, list->tail, data);

    list->tail->previous = head;
    // set new head
    list->head = head;
    return 0;
  }

  // head and tail exist, change head
  // update the formerly first node
  Node *new_head = _nodeCreate(NULL, list->head, data);
  list->head->previous = new_head;
  list->head = new_head;

  return 0;
}

/* Remove an item from the list according to its index. Returns -1 if an error is encountered or the size of the data in case of success.
If destination is NULL, no values is read. */
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

  _nodeDelete(current);
  list->length--;

  return sizeof(*destination);
}

/* Remove an item from the list according to its value. Returns -1 if an error is encountered or the size of the data in case of success. */
int listRemoveItemByValue(List *list, Data *oldvalue)
{
  int index;
  index = _findNodeIndexByValue(list, oldvalue);

  if (index == -1)
    return -1;

  return listRemoveItem(list, NULL, index);
}

/* Pop the list, removing its last item. Returns -1 if an error is encountered or the size of the data in case of success. */
int listPop(List *list, Data *last)
{
  if (listRemoveItem(list, last, list->length - 1) != -1)
    return sizeof(*last);

  return -1;
}

/* Unshift the list, removing its first item. Returns -1 if an error is encountered or the size of the data in case of success. */
int listUnshift(List *list, Data *first)
{
  if (listRemoveItem(list, first, 0) != -1)
    return sizeof(*first);

  return -1;
}

/* Replace the data in an item according to its value. Returns its index if found, -1 in case of an error. */
int listReplaceItemByValue(List *list, Data *oldvalue, Data *newvalue)
{
  int index;
  index = _findNodeIndexByValue(list, oldvalue);

  if (index == -1)
    return -1;

  return listReplaceItem(list, newvalue, index);
}

/* Replace the data in an item according to its index. Returns its size if found, -1 in case of an error. */
int listReplaceItem(List *list, Data *newvalue, int index)
{
  Node *node;
  node = _findNodeByIndex(list, index);

  if (node == NULL)
    return -1;

  return _nodeSetData(node, newvalue);
}

/* Replces all occurrences of a certain value from the list. Return number of removed items. If remove count is -1,
replace all occurencies. */
int listCountReplace(List *list, Data *oldvalue, Data *newvalue, int replace_count)
{
  int count = 0;

  while (count < replace_count || replace_count == -1)
  {

    if (listReplaceItemByValue(list, oldvalue, newvalue) == -1)
      break;

    count++;
  }

  return count;
}

/* Remove all occurrences of a certain value from the list. Return number of removed items. If remove count is -1,
removes all occurencies. */
int listCountRemove(List *list, Data *data, int remove_count)
{
  int count = 0;

  while (count < remove_count || remove_count == -1)
  {

    if (listRemoveItemByValue(list, data) == -1)
      break;

    count++;
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
  Node *new = _nodeCreate(previous, next, data);
  previous->next = new;
  next->previous = new;
  list->length++;

  return position;
}

/* Check if data is in list. If found, returns its position. If not found, returns -1 */
int dataInList(List *list, Data *data)
{
  return _findNodeIndexByValue(list, data);
}

/* Make an array out of the list. Returns the size of the array */
int listToArray(List *list, Data *array)
{
  int length;
  Node *current;

  length = listGetSize(list);
  current = list->head;

  for (int i = 0; i < length; i++)
  {
    if (current == NULL)
      return i;

    _nodeGetData(current, &array[i]);
    current = current->next;
  }

  return length;
}

/* Print the list in a readable manner */
void printList(List *list, char *end)
{

  Node *current = list->head;
  int index = 0;

  while (current != NULL)
  {
    _nodePrint(current, index);

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
    _nodePrint(current, index);

    current = current->previous;
    index--;
  }

  printf("%s", end);
}

/* Return the length of the list */
int listGetSize(List *list)
{
  return list->length;
}

/* Returns an iterator for the list. If index == 0, the iterator starts at the head of the list.
If index == -1, it starts at the end of the list. Otherwise, it starts at the corresponding index of the list */
Iterator *iteratorCreate(List *list, int index)
{
  Iterator *new = NULL;
  new = (Iterator *)malloc(sizeof(Iterator));

  if (index > list->length - 1)
    index = list->length - 1;

  if (index == 0)
  {
    new->current = list->head;
    new->next = list->head->next;
    new->previous = NULL;
  }
  else if (index == -1)
  {
    new->current = list->tail;
    new->previous = list->tail->previous;
    new->next = NULL;
  }
  else
  {
    Node *node;
    node = _findNodeByIndex(list, index);
    new->current = node;
  }

  return new;
}

/* Delete an iterator. */
void iteratorDelete(Iterator *it)
{
  free(it);
  return;
}

/* Tells if the iterator has reached the end */
int iteratorEnded(Iterator *it)
{
  if (it->next == NULL)
    return 1;

  return 0;
}

/* Tells if the iterator has reached the start */
int iteratorStarted(Iterator *it)
{
  if (it->previous == NULL)
    return 1;

  return 0;
}

/* Moves the iterator forward by an item. Returns -1 if the list ended. */
int iteratorNext(Iterator *it)
{
  if (it->next != NULL)
  {
    it->current = it->next;
    it->previous = it->current->previous;
    it->next = it->current->next;
    return 0;
  }

  return -1;
}

/* Moves the iterator backward by an item. Returns -1 if the list ended. */
int iteratorPrevious(Iterator *it)
{
  if (it->previous != NULL)
  {
    it->current = it->previous;
    it->previous = it->current->previous;
    it->next = it->current->next;
    return 0;
  }

  return -1;
}

/* Loads the current value of the iterator. Returns -1 in case of error, the size of destination in case of success. */
int iteratorGetData(Iterator *it, Data *destination)
{
  return _nodeGetData(it->current, destination);
}