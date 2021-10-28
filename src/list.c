#include "list.h"

/**
 * @internal
 * @brief Internal function. Creates a node with link to previous and next node.
 * 
 * @param previous Pointer to previous node, can be NULL.
 * @param next Pointer to next node, can be NULL.
 * @param data Data to be put in the new node.
 * @return Node* the new node, already linked.
 */
Node *_nodeCreate(Node *previous, Node *next, Data *data)
{
  Node *new = NULL;
  new = (Node *)malloc(sizeof(Node));

  new->previous = previous; // can be NULL
  new->next = next;         // can be NULL
  new->data = *data;

  return new;
}

/**
 * @internal
 * @brief Internal function. Deletes a node.
 * 
 * @param node Pointer to the node to be deleted.
 */
void _nodeDelete(Node *node)
{
  free(node);
  return;
}

/**
 * @internal
 * @brief Internal function. Finds a node by its index.
 * 
 * @param list List containing the node to look for.
 * @param index Index of the node.
 * @return Node* 
 */
Node *_findNodeByIndex(List *list, int index)
{
  if (index >= listGetSize(list))
    return NULL;

  Node *current;
  int count, increment;

  if (index > listGetSize(list) / 2)
  {
    // if the node is closer to then end than to the start, go backwards from the tail
    current = list->tail;
    count = listGetSize(list) - 1;
    increment = -1;
  }
  else
  {
    // otherwise, go forward from the head
    current = list->head;
    count = 0;
    increment = 1;
  }

  // loop over the list
  while (current != NULL)
  {
    if (index == count)
      return current;

    count += increment;

    // increment according to direction
    if (increment == 1)
      current = current->next;
    else
      current = current->previous;
  }

  return NULL;
}

/**
 * @internal
 * @brief Internal function. Finds a node in a list by its value.
 * 
 * @param list List containing the node to look for.
 * @param data Value of the node.
 * @return Node* 
 */
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

/**
 * @internal 
 * @brief Finds a node index in a list by its value.
 * 
 * @param list List containing the node to look for.
 * @param data Pointer containing value of the node.
 * @return int Index of the nome.
 */
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

/**
 * @internal
 * @brief Prints the value of the node.
 * 
 * @param node Node to be printed.
 * @param index Index of the node.
 */
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

/**
 * @internal
 * @brief Gets the data in an item.
 * 
 * @param node Node from which the data will be taken.
 * @param destination Pointer to the variable where data will be saved. If it's NULL, no value is read.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @internal
 * @brief Sets the data in an item.
 * 
 * @param node Node in which the data will be written.
 * @param data Pointer to the source variable. If it's NULL, no value is written.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
int _nodeSetData(Node *node, Data *data)
{
  if (node != NULL)
  {
    node->data = *data;
    return sizeof(*data);
  }

  return -1;
}

/**
 * @internal
 * @brief Retrieves a node from a list by its index.
 * 
 * @param list List containing the node to look for.
 * @param destination Pointer to the variable where data will be saved.
 * @param index Index of the node in the list.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @internal
 * @brief Retrieves a node from a list by its value.
 * 
 * @param list List containing the node to look for.
 * @param destination Pointer to the variable where data will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @internal
 * @brief Swaps two nodes.
 * 
 * @param n1 Pointer to the first node to be swapped.
 * @param n2 Pointer to the second node to be swapped.
 */
void _listSwapNodes(Node *n1, Node *n2)
{

#ifdef STRINGLIST
  Data temp[STRINGSIZE];
  strcpy(temp, n1->data);
  strcpy(n1->data, n2->data);
  strcpy(n2->data, temp);
#else
  Data temp;
  temp = n1->data;
  n1->data = n2->data;
  n2->data = temp;

#endif
}

/**
 * @brief Creates and returns a list.
 * 
 * @return List* The new (empty) list.
 */
List *listCreate()
{
  List *list = malloc(sizeof(List));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}

/**
 * @brief Deletes a list and all of its nodes.
 * 
 * @param list The list to be deleted.
 */
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

/**
 * @brief Retrieves an item in a list by its index.
 * 
 * @param list List containing the data to look for.
 * @param destination Pointer to the variable where data will be saved.
 * @param index -1 if an error is encountered; size of the items otherwise.
 * @return int 
 */
int listGetItem(List *list, Data *destination, int index)
{
  Node *node = _findNodeByIndex(list, index);
  if (node == NULL)
    return -1;

  if (_nodeGetData(node, destination))
    return sizeof(*destination);

  return -1;
}

/**
 * @brief Gets the first item in a list.
 * 
 * @param list List containing the data to look for.
 * @param destination Pointer to the variable where data will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @brief Gets the last item in a list.
 * 
 * @param list List containing the data to look for.
 * @param destination Pointer to the variable where data will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @brief Appends an item to the list.
 * 
 * @param list List where the items will be appended.
 * @param data Data to append.
 * @return int New length of the list.
 */
int listAppend(List *list, Data *data)
{
  if (list->head == NULL)
  {
    // no head, create one
    Node *head = _nodeCreate(NULL, NULL, data);
    // update list
    list->head = head;
  }
  else if (list->tail == NULL)
  {
    // no tail, create one
    // there's only one item in the list, the head
    Node *tail = _nodeCreate(list->head, NULL, data);
    // update list and tail
    list->head->next = tail;
    list->tail = tail;
  }
  else
  { // head and tail exist, change tail
    // update the formerly last node
    Node *new_tail = _nodeCreate(list->tail, NULL, data);
    list->tail->next = new_tail;
    list->tail = new_tail;
  }

  list->length++;
  return list->length;
}

/**
 * @brief Adds an item in front of the list.
 * 
 * @param list List where the items will be appended.
 * @param data Data to prepend.
 * @return int New length of the list.
 */
int listPrepend(List *list, Data *data)
{
  if (list->head == NULL)
  {
    // no head, create one
    Node *head = _nodeCreate(NULL, NULL, data);
    // update list
    list->head = head;
  }
  else if (list->tail == NULL)
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
  }
  else
  {
    // head and tail exist, change head
    // update the formerly first node
    Node *new_head = _nodeCreate(NULL, list->head, data);
    list->head->previous = new_head;
    list->head = new_head;
  }

  list->length++;
  return list->length;
}

/**
 * @brief Removes an item from the list according to its index.
 * 
 * @param list List from where the data will be removed.
 * @param destination Pointer to the variable where the data will be saved. If NULL, nothing will be saved.
 * @param index Index of the items that will be removed.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
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

/**
 * @brief Removes an item from the list according to its value.
 * 
 * @param list List from where the data will be removed.
 * @param oldvalue Value of the items that will be removed.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
int listRemoveItemByValue(List *list, Data *oldvalue)
{
  int index;
  index = _findNodeIndexByValue(list, oldvalue);

  if (index == -1)
    return -1;

  return listRemoveItem(list, NULL, index);
}

/**
 * @brief Pops the list, removing its last item.
 * 
 * @param list List that will be popped.
 * @param last Pointer to the variable where the data will be saved. If NULL, nothing will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
int listPop(List *list, Data *last)
{
  if (listRemoveItem(list, last, list->length - 1) != -1)
    return sizeof(*last);

  return -1;
}

/**
 * @brief Unshifts the list, removing its first item.
 * 
 * @param list List where the items will be added.
 * @param first Pointer to the variable where the data will be saved. If NULL, nothing will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
int listUnshift(List *list, Data *first)
{
  if (listRemoveItem(list, first, 0) != -1)
    return sizeof(*first);

  return -1;
}

/**
 * @brief Swaps two items in a list according to their index.
 * 
 * @param list List where the items will be swapped.
 * @param first_index Index of the first item.
 * @param second_index Index of the second item.
 * @return int 0 if successfull, -1 of an error is encountered.
 */
int listSwap(List *list, int first_index, int second_index)
{
  // check if the indexes are in bound
  if (first_index > list->length || second_index > list->length)
    return -1;
  // fetch the nodes
  Node *n1, *n2;
  n1 = _findNodeByIndex(list, first_index);
  n2 = _findNodeByIndex(list, second_index);
  // swap two nodes
  _listSwapNodes(n1, n2);
  return 0;
}

/**
 * @brief Replaces the data in an item according to its value.
 * 
 * @param list List where the items will be replaced.
 * @param oldvalue Old value of the items.
 * @param newvalue New value of the items.
 * @return int Index of the item if found, -1 in case of an error.
 */
int listReplaceItemByValue(List *list, Data *oldvalue, Data *newvalue)
{
  int index;
  index = _findNodeIndexByValue(list, oldvalue);

  if (index == -1)
    return -1;

  return listReplaceItem(list, newvalue, index);
}

/**
 * @brief Replaces the data in an item according to its index.
 * 
 * @param list List where the items will be replaced.
 * @param newvalue New value of the items.
 * @param index Index of the old items.
 * @return int Size of the new item if found; -1 otherwise.
 */
int listReplaceItem(List *list, Data *newvalue, int index)
{
  Node *node;
  node = _findNodeByIndex(list, index);

  if (node == NULL)
    return -1;

  return _nodeSetData(node, newvalue);
}

/**
 * @brief Replaces all occurrences of a certain value from the list.
 * 
 * @param list List where the items will be replaced.
 * @param oldvalue Old value of the items.
 * @param newvalue New value of the items.
 * @param replace_count Number of times the items shall be replaced. If -1, all matching items will be replaced.
 * @return int Number of replaced items.
 */
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

/**
 * @brief Removes all occurrences of a certain value from the list.
 * 
 * @param list List where the items will be removed.
 * @param data Value of the items that will be removed.
 * @param remove_count Number of times the items shall be replaced. If -1, all matching items will be removed.
 * @return int Number of removed items.
 */
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

/**
 * @brief Adds item to the list.
 * 
 * @param list List where the items will be added.
 * @param data Value of the items that will be removed.
 * @param index Position of the new item.
 * @return int Length of the list.
 */
int listAddItem(List *list, Data *data, int index)
{
  if (index == 0)
  {
    // add head
    return listPrepend(list, data);
  }

  if (index > list->length - 1)
  {
    // add tail
    return listAppend(list, data);
  }

  // add new node
  Node *previous = _findNodeByIndex(list, index - 1);
  Node *next = _findNodeByIndex(list, index);
  Node *new = _nodeCreate(previous, next, data);
  previous->next = new;
  next->previous = new;
  list->length++;

  return list->length;
}

/* . If found, returns its position. If not found, returns -1 */
/**
 * @brief Checks if item is in list.
 * 
 * @param list List where the items will be searched.
 * @param data Value of the item that will be searched.
 * @return int Position of the item if found; -1 otherwise.
 */
int dataInList(List *list, Data *data)
{
  return _findNodeIndexByValue(list, data);
}

/**
 * @brief Makes an array out of the list.
 * 
 * @param list List that will be converted.
 * @param array Pointer to the array where the items will be put.
 * @return int The size of the array.
 */
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

/**
 * @brief Prints the list in a readable manner.
 * 
 * @param list List that will be printed.
 * @param end End of line separator.
 */
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

/**
 * @brief Prints the list in a readable manner, in reverse.
 * 
 * @param list List that will be printed.
 * @param end End of line separator.
 */
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

/**
 * @brief Returns the length of the list.
 * 
 * @param list List whose the size will be returned.
 * @return int Length of the list.
 */
int listGetSize(List *list)
{
  return list->length;
}

/**
 * @brief Sorts a list using quicksort. Does not work with CUSTOMTYPE.
 * 
 * @param list List that will be sorted.
 * @param reverse Order of the sorting. If is 0, the list is sorted lowest to highest. If 1, it's sirted highest to lowest.
 * @return int 0 in case of success; -1 otherwise.
 */
int listSort(List *list, int reverse)
{
#ifdef CUSTOMTYPE
  return -1;
#endif

  if (reverse != 0 && reverse != 1)
    return -1;

  int sorted;
  Iterator *it;
  Node *current, *next;

  sorted = 0;
  while (sorted == 0)
  {
    sorted = 1;
    it = iteratorCreate(list, 0);
    // evaluate end condition
    while (!iteratorEnded(it) && it->index < list->length - 1)
    {
      current = it->current;
      next = it->next;

#ifdef STRINGLIST
      int comparison = reverse ? current->data[0] <next->data[0]) : current->data[0] > next->data[0]);
      if (comparison)
#else
      int comparison = reverse ? current->data < next->data : current->data > next->data;
      if (comparison)
#endif
      {
        sorted = 0;
        _listSwapNodes(current, next);
      }

      iteratorNext(it);
    }
    iteratorDelete(it);
  }

  return 0;
}

/**
 * @brief Shuffles a list using Fisher-Yates algorithm.
 * 
 * @param list List that will be shuffled.
 */
void listShuffle(List *list)
{
  srand(time(NULL));

  for (int i = list->length - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    listSwap(list, i, j);
  }
}

/**
 * @brief Creates an iterator for the list.
 * 
 * @param list List from where the iterator will be extracted.
 * @param index Starting index of the iterator. If 0, starts at the head of the list. If -1, starts at its end.
 * @return Iterator* The new iterator.
 */
Iterator *iteratorCreate(List *list, int index)
{
  Iterator *new = NULL;
  new = (Iterator *)malloc(sizeof(Iterator));

  // modulo index to list length
  if (index > list->length - 1)
    index = list->length - 1;

  if (index == 0)
  {
    // point to first item
    new->current = list->head;
    new->next = list->head->next;
    new->previous = NULL;
  }
  else if (index == -1)
  {
    // point to last item
    new->current = list->tail;
    new->previous = list->tail->previous;
    new->next = NULL;
  }
  else
  {
    // find a node
    Node *node;
    node = _findNodeByIndex(list, index);
    new->current = node;
  }

  // set current index
  new->index = index;

  return new;
}

/**
 * @brief Deletes an iterator.
 * 
 * @param it Iterator that will be deleted.
 */
void iteratorDelete(Iterator *it)
{
  free(it);
  return;
}

/**
 * @brief Checks if the iterator has reached the end.
 * 
 * @param it Iterator to check.
 * @return int 1 if true, 0 if false.
 */
int iteratorEnded(Iterator *it)
{
  if (it->next == NULL)
    return 1;

  return 0;
}

/**
 * @brief Checks if the iterator has reached the start.
 *
 * @param it Iterator to check.
 * @return int 1 if true, 0 if false.
 */
int iteratorStarted(Iterator *it)
{
  if (it->previous == NULL)
    return 1;

  return 0;
}

/**
 * @brief Moves the iterator forward by an item.
 * 
 * @param it Iterator to move.
 * @return int 0 if it moved successfully, -1 if the list is ended.
 */
int iteratorNext(Iterator *it)
{
  if (it->next != NULL)
  {
    it->current = it->next;
    it->previous = it->current->previous;
    it->next = it->current->next;
    it->index++;
    return 0;
  }

  return -1;
}

/**
 * @brief Moves the iterator backward by an item.
 * 
 * @param it Iterator to move.
 * @return int 0 if it moved successfully, -1 if the list is ended.
 */
int iteratorPrevious(Iterator *it)
{
  if (it->previous != NULL)
  {
    it->current = it->previous;
    it->previous = it->current->previous;
    it->next = it->current->next;
    it->index--;
    return 0;
  }

  return -1;
}

/**
 * @brief Loads the current value of the iterator.
 * 
 * @param it Iterator to get the value of.
 * @param destination Pointer to the variable where the data will be saved. If NULL, nothing will be saved.
 * @return int -1 if an error is encountered; size of the items otherwise.
 */
int iteratorGetData(Iterator *it, Data *destination)
{
  return _nodeGetData(it->current, destination);
}