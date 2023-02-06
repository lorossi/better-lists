# Better Lists

A simple implementation of doubly linked lists in C.

I made this mostly because I was bored, but I went on because I took this as a challenge. When I first studied C at university (*it was my first year!)*, I couldn't get linked lists to work. It's a very satisfying goal to reach, about 5 years *(and a degree)* later.

Each function is well commented, and in this readme I have included some examples. The code is fully tested against memory leaks using Valgrind. Inside the `tests.c` file I have implemented a simple test suite using the `assert.h` library. It's not pretty, but it works.

[Check the documentation here](https://lorossi.github.io/better-lists/html/) or scroll a little bit below to see some examples in action.

## Functions in library

### Lists related

```C
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
int listSort(List *list, int reverse);
int listShuffle(List *list);
```

#### Helper functions

```C
int listGetInt(List *list, int index);
char listGetChar(List *list, int index);
float listGetFloat(List *list, int index);
double listGetDouble(List *list, int index);
char *listGetString(List *list, int index);
void *listGetPointer(List *list, int index);
```

### Iterator related

```C
// Iterator related functions
Iterator *iteratorCreate(List *list, int start);
void iteratorDelete(Iterator *it);
int iteratorEnded(Iterator *it);
int iteratorStarted(Iterator *it);
int iteratorNext(Iterator *it);
int iteratorPrevious(Iterator *it);
int iteratorGetNode(Iterator *it, Node *destination);
int iteratorGetData(Iterator *it, union Data *destination);
int iteratorSetData(Iterator *it, union Data *new_value);
int iteratorGetIndex(Iterator *it);
```

## Data structures in library

### Union data

```C
union Data
{
  int i;
  char c;
  float f;
  double d;
  char *s;
  void *p;
};
```

The `union Data` represents contained in each node of the list.
It can contain an integer, a char, a float, a double, a string or a void pointer.

To access the value associated to a `Data union`, simply access the field associated to the desired type.

```C
// define a generic union Data
union Data data;
// get an integer 
int i_data = data.i;
// get a char
char c_data = data.c;
// get a float
float f_data = data.f;
// get a double
double d_data = data.d;
// get a string
char *s_data = data.s;
// get a void pointer
void *p_data = data.p;
```

### List types

```C
typedef enum
{
  INTEGER,
  CHAR,
  FLOAT,
  DOUBLE,
  STRING,
  POINTER,
} list_type;
```

The `list_type` enum represents the type of the list. It can be an integer list, a char list, a float list, a double list, a string list or a pointer list.

The `list_type` is passed in the `listCreate` function to create a list of the desired type.

### Nodes

```C
typedef struct node
{
  union Data data;       /**< Data contained in node. */
  struct node *previous; /**< Pointer to previous node */
  struct node *next;     /**< Pointer to next node */
} Node;
```

The `Node` struct represents a node of the list. It contains a `union Data` field, a pointer to the previous node and a pointer to the next node.

### List

```C
typedef struct list
{
  Node *head;     /**< Pointer to first node */
  Node *tail;     /**< Pointer to last node */
  int length;     /**< Number of nodes in list */
  list_type type; /**< Type of data contained in node. */
} List;
```

The `List` struct represents a list. It contains a pointer to the first node, a pointer to the last node, the number of nodes in the list and the type of the list.

### Iterator

```C
typedef struct iterator
{
  struct node *previous; /**< Pointer to previous node */
  struct node *next;     /**< Pointer to next node */
  struct node *current;  /**< Pointer to current node */
  int index;
} Iterator;
```

## Examples

### List creation

While creating a list, you have to specify the type of the list. The type can be an integer list, a char list, a float list, a double list, a string list or a pointer list (as explained in [List types](#list-types)).

```C
// create int list
List *int_list = listCreate(INTEGER);
// create char list
List *char_list = listCreate(CHAR);
// create float list
List *float_list = listCreate(FLOAT);
// create double list
List *double_list = listCreate(DOUBLE);
// create string list
List *string_list = listCreate(STRING);
// create pointer list
List *pointer_list = listCreate(POINTER);
```

### List deletion

When you are done with a list, you have to delete it to free the memory calling the `listDelete` function.

```C
List *l = listCreate(INTEGER);
/* ... */
listDelete(l);
```

### Adding items to list

Items can be added to a list via the functions:

- `listPush`: add an item to the end of the list
- `listPrepend`: add an item to the beginning of the list
- `listAddItem`: add an item to the list at a specific index

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data data;
// add an item to the end of the list
data.i = 1;
listPush(l, &data);
// add an item to the beginning of the list
data.i = 2;
listPrepend(l, &data);
// add an item to the list at a specific index
data.i = 3;
listAddItem(l, &data, 1);
```

### Getting items from list

Items can be retrieved from a list via the functions:

- `listGetItem`: get an item from the list at a specific index
- `listGetFirstItem`: get the first item from the list
- `listGetLastItem`: get the last item from the list

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data data;
// get an item from the list at a specific index
listGetItem(l, &data, 1);
// get the first item from the list
listGetFirstItem(l, &data);
// get the last item from the list
listGetLastItem(l, &data);
```

### Getting items from list - alternative

Items can be retrieved from a list via the functions:

- `listGetInt`: get an integer from the list at a specific index
- `listGetChar`: get a char from the list at a specific index
- `listGetFloat`: get a float from the list at a specific index
- `listGetDouble`: get a double from the list at a specific index
- `listGetString`: get a string from the list at a specific index
- `listGetPointer`: get a pointer from the list at a specific index

With all these functions, the value is returned directly, without the need to pass a `union Data` variable.

```C
// create a list
List *l = listCreate(INTEGER);
// get an integer from the list at a specific index
int i_data = listGetInt(l, 1);
// delete the list
listDelete(l);

// create a list
l = listCreate(CHAR);
// get a char from the list at a specific index
char c_data = listGetChar(l, 1);
// delete the list
listDelete(l);

// create a list
l = listCreate(FLOAT);
// get a float from the list at a specific index
float f_data = listGetFloat(l, 1);
// delete the list
listDelete(l);

// create a list
l = listCreate(DOUBLE);
// get a double from the list at a specific index
double d_data = listGetDouble(l, 1);
// delete the list
listDelete(l);

// create a list
l = listCreate(STRING);
// get a string from the list at a specific index
char *s_data = listGetString(l, 1);
// delete the list
listDelete(l);

// create a list
l = listCreate(POINTER);
// get a pointer from the list at a specific index
void *p_data = listGetPointer(l, 1);
// delete the list
listDelete(l);
```

### Replacing items in list

Items can be replaced in a list via the functions:

- `listReplaceItem`: replace an item in the list at a specific index
- `listReplaceItemByValue`: replace an item in the list by value
- `listCountReplace`: replace a number of items in the list by value

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data new_data, old_data;
// maximum number of items to replace
int max_count = 5;
// replace an item in the list at a specific index
new_data.i = 1;
listReplaceItem(l, &new_data, 1);
// replace an item in the list by value
new_data.i = 2;
old_data.i = 1;
listReplaceItemByValue(l, &old_data, &new_data);
// replace a number of items in the list by value
new_data.i = 3;
old_data.i = 2;
listCountReplace(l, &old_data, &new_data, max_count);
```

### Removing items from list

Items can be removed from a list via the functions:

- `listRemoveItem`: remove an item from the list at a specific index
- `listRemoveItemByValue`: remove an item from the list by value
- `listCountRemove`: remove a number of items from the list by value
- `listPop`: remove the last item from the list
- `listShift`: remove the first item from the list

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data data;
// maximum number of items to remove
int max_count = 5;
// remove an item from the list at a specific index
listRemoveItem(l, 1);
// remove an item from the list by value
data.i = 1;
listRemoveItemByValue(l, &data);
// remove a number of items from the list by value
data.i = 2;
listCountRemove(l, &data, max_count);
// remove the last item from the list
listPop(l);
// remove the first item from the list
listShift(l);
```

### Support functions

The library provides some support functions:

- `listGetSize`: get the length of the list
- `listCountItem`: count the number of items in the list
- `listSwap`: swap two items in the list
- `dataInList`: check if an item is in the list
- `listSort`: sort the list
- `listShuffle`: shuffle the list
- `printList`: print the list
- `listToArray`: convert the list to an array

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data data;
// get the length of the list
int size = listGetSize(l);
// count the number of items in the list
int count = listCountItem(l, &data);
// swap two items in the list
listSwap(l, 1, 2);
// check if an item is in the list
int in_list = dataInList(l, &data);
// sort the list
listSort(l, 0);
// sort the list in reverse order
listSort(l, 1);
// shuffle the list
listShuffle(l);
// print the list, each item on a new line
printList(l, "\n");
// print the list, each item separated by a space
printList(l, " ");
// convert the list to an array
int *array = listToArray(l);
```

### Iterating over list

The library provides a structure to iterate over a list (`Iterator` struct) and a set of functions to use it:

- `IteratorCreate`: create an iterator
- `IteratorDelete`: delete an iterator
- `IteratorNext`: move the iterator to the next item
- `IteratorPrevious`: move the iterator to the previous item
- `IteratorGetNode`: get the node pointed by the iterator
- `IteratorGetData`: get the data pointed by the iterator
- `IteratorSetData`: set the data pointed by the iterator
- `IteratorGetIndex`: get the index of the node pointed by the iterator

```C
// create a list
List *l = listCreate(INTEGER);
// create a union Data
union Data data;
// create an iterator
Iterator *it = IteratorCreate(l);
// move the iterator to the next item
IteratorNext(it);
// move the iterator to the previous item
IteratorPrevious(it);
// get the node pointed by the iterator
Node *node = IteratorGetNode(it);
// get the data pointed by the iterator
IteratorGetData(it, &data);
// set the data pointed by the iterator
data.i = 1;
IteratorSetData(it, &data);
// get the index of the node pointed by the iterator
int index = IteratorGetIndex(it);
// delete the iterator
IteratorDelete(it);
```

The list can be iterated over using the following code:

```C
// create a list
List *l = listCreate(INTEGER);
/* fill the list */
// declare an iterator
Iterator *it;
// start the loop
for (it = IteratorCreate(l, 0); !iteratorEnded(it); IteratorNext(it)) {
    // get the data pointed by the iterator
    IteratorGetData(it, &data);
    /* do something with the data */
}
// delete the iterator
IteratorDelete(it);
```

The list can be iterated over in reverse order using the following code:

```C
// create a list
List *l = listCreate(INTEGER);
/* fill the list */
// declare an iterator
Iterator *it;
// start the loop
for (it = IteratorCreate(l, -1); !iteratorStarted(it); IteratorPrevious(it)) {
    // get the data pointed by the iterator
    IteratorGetData(it, &data);
    /* do something with the data */
}
// delete the iterator
IteratorDelete(it);
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.
