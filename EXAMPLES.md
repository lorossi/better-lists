# Code examples

## Iterators

While navigating a linked list by getting the item at i-th position, all the nodes from the head to the i-th element must be navigated through. In order to save some computation time and speed, I have introduced the *iterators*. They keep track of the current, previous and next node in the list, thus allowing the programmer to make efficient iterations over lists. Check the examples to learn how to use them.

## Functions

### Lists related

``` C
List *listCreate();
void listDelete(List *list);
int listGetItem(List *list, union Data *destination, int index);
int listGetFirstItem(List *list, union Data *destination);
int listGetLastItem(List *list, union Data *destination);
int listAddItem(List *list, union Data *destination, int position);
int listAppend(List *list, union Data *destination);
int listPrepend(List *list, union Data *destination);
int listReplaceItem(List *list, union Data *new_value, int index);
int listReplaceItemByValue(List *list, union Data *old_value, union Data *new_value);
int listCountReplace(List *list, union Data *old_value, union Data *new_value, int replace_count);
int listRemoveItem(List *list, union Data *destination, int index);
int listRemoveItemByValue(List *list, union Data *old_value);
int listCountRemove(List *list, union Data *old_value, int remove_count);
int listPop(List *list, union Data *last);
int listShift(List *list, union Data *destination);
int dataInList(List *list, union Data *destination);
int listToArray(List *list, union Data *array);
void printList(List *list, char *end);
void printListReverse(List *list, char *end);
int listGetSize(List *list);
```

### Iterators related

``` C
Iterator *iteratorCreate(List *list, int start);
void iteratorDelete(Iterator *it);
int iteratorEnded(Iterator *it);
int iteratorStarted(Iterator *it);
int iteratorNext(Iterator *it);
int iteratorPrevious(Iterator *it);
int iteratorGetData(Iterator *it, union Data *destination);
```

## Example

### List creation

``` C
List *l;
// memory is allocated
l = createList();
// IMPORTANT, always delete lists when not needed any more
deleteList(l);
```

### Getting list size

``` C
List *l;
int n;
// list creation and population not in this example
n = listGetSize(l);
printf("List is %i items long\n", n);
```

### Adding data to a list

``` C
List *l;
Data d;
l = createList();

// appending to list
for (int i = 0; i < 20; i++)
{
  d = i;
  listAppend(l, &d);
}
// prepending to list
for (int i = 0; i < 20; i++)
{
  d = 20 - i - 1;
  listPrepend(l, &d);
}
// add 99 at position 4
d = 99;
listAddItem(l, &d, 4);
// always delete the list to free the space
deleteList(l)
```

### Replacing items in a list

``` C
List *l;
Data d, old, new;
// list creation and population not in this example
// replace item at index 6
d = 40;
listReplaceItem(l, &d, 6);
// replace the first item with value 37 in the list with 8
old = 37;
new = 8;
listReplaceItemByValue(l, &old, &new);
// replace the first 3 instances of 8 with 9;
old = 8;
new = 9;
listCountReplace(l, &old, &new, 3);
// replace all the instances of 9 with 10
old = 9;
new = 10;
listCountReplace(l, &old, &new, -1); 
```

### Getting items from the list

``` C
List *l;
Data d;
// list creation and population not in this example
// get item at index 3
listGetItem(l, &d, 3);
// now d contains the item in position 3 of the list
// to get the first and last item, use the proper function to save time
listGetFirstItem(l, &d);
listGetLastItem(l, &d);
```

### Removing items from the list

``` C
List *l;
Data d;
// list creation and population not in this example
// remove first item and place it in d
listShift(l, &d); 
// remove last item without saving it
listPop(l, NULL);
// remove item at position 11
listRemoveItem(l, 11);
// remove the first occurrence of 4 from list
listRemoveItemByValue(l, 4);
// remove the first 5 occurrences of 77 from list
listCountRemove(l, 5);
// remove all occurrences of 1 from list
listCountRemove(l, -1);
```

### Swap items

``` C
List *l;
Data d;
// list creation and population not in this example
listSwap(l, 0, 2); // swaps item in position 0 and 2
```

### Sorting list

``` C
List *l;
// list creation and population not in this example
listSort(l, 0); // sorts lowest to highest
listSort(l, 1); // sorts highest to lowest
```

### Shuffle list

``` C
List *l;
// list creation and population not in this example
listShuffle(l);
```

### Check if an item is in list

``` C
List *l;
Data d;
// list creation and population not in this example
d = 5;
if (dataInList(l, &d) == 1)
  printf("%i in List\n");
```

### Printing list

``` C
List *l;
char end;
// list creation and population not in this example
end = "\n";
// print the list from start to end
printList(l, end);
// print the list backwards
printListReverse(l, end);
```

### Converting list into array

``` C
List *l;
Data arr[listGetSize(l)];
// list creation and population not in this example
listToArray(l, a);
```

### Using iterators

``` C
List *l;
Data d;
Iterator it;
// forward iteration
for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
{
  iteratorGetData(it, &d);
  printf("Next item: %i\n", d);
}
// always delete the iterator to free the allocated space
iteratorDelete(it);
// backward iteration
for (it = iteratorCreate(l, -1); !iteratorStarted(it); iteratorPrevious(it))
{
  iteratorGetData(it, &d);
  printf("Next item: %i\n", d);
}
iteratorDelete(it);
```
