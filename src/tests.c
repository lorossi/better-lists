#include <assert.h>
#include "list.h"

// tested against memory leaks using VALGRING
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

const int LIST_TEST_SIZE = 1e5;

void printGreen(char *s)
{
  printf("\x1b[92;1m%s\x1b[0m\n", s);
}

void printYellow(char *s)
{
  printf("\x1b[93;1m%s\x1b[0m\n", s);
}

void printRed(char *s)
{
  printf("\x1b[91;1m%s\x1b[0m\n", s);
}

void printSuccess(char *s)
{
  printf("\x1b[92;1;5m%s\x1b[0m\n", s);
}

void quickPopulate(List *l)
{
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    Data d = i;
    listAppend(l, &d);
  }
}

int main()
{
  Data d, t;
  List *l;
  // tests the empty list
  printYellow("Testing empty list...");
  // create list and try to get items
  l = listCreate();
  assert(listGetLength(l) == 0);
  assert(listGetFirstItem(l, &d) == -1);
  assert(listGetLastItem(l, &d) == -1);
  assert(listGetItem(l, &d, 10) == -1);
  // now try to remove items
  assert(listRemoveItem(l, &d, 10) == -1);
  assert(listFindAndRemoveItems(l, &d, 10) == 0);
  assert(listPop(l, &d) == -1);
  assert(listUnshift(l, &d) == -1);
  listDelete(l);
  printGreen("Passed.");

  // test list population TODO
  printYellow("Testing list population...");
  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    d = i;
    // append to list, check size
    assert(listAppend(l, &d) == i);
    assert(listGetLength(l) == i + 1);
    // check if item was correctly added
    assert(listGetItem(l, &t, i) == sizeof(t));
    assert(t == i);
  }
  // get items from list
  assert(listGetItem(l, NULL, 0) != 0);
  assert(listGetFirstItem(l, NULL) != 0);
  assert(listGetLastItem(l, NULL) != 0);
  assert(itemInList(l, &d) != 0);
  // check if item is is list
  t = LIST_TEST_SIZE + 1;
  assert(itemInList(l, &t) == -1);
  t = 0;
  assert(itemInList(l, &t) != -1);
  // delete list and check if the item is in it
  listDelete(l);

  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    Data d = i;
    // append data to list
    assert(listPrepend(l, &d) == 0);
    assert(listGetLength(l) == i + 1);
    Data t;
    // check if appended data is inserted correctly
    assert(listGetItem(l, &t, 0) == sizeof(t));
    assert(t == i);
  }
  // try replacing items
  d = 100;
  assert(listReplaceItem(l, &d, 0) != 0);
  // now check if item was replaced correctly
  assert(listGetFirstItem(l, &t) != 0);
  assert(d == t);
  // try replacing in different places
  d = LIST_TEST_SIZE / 4;
  assert(listReplaceItem(l, &d, 0) != 0);
  // now check if item was replaced correctly
  assert(listGetFirstItem(l, &t) != 0);
  assert(d == t);
  // try replacing item by value
  t = 0;
  assert(listReplaceItemByValue(l, &d, &t) != 0);
  listDelete(l);

  // create list and add item in front
  l = listCreate();
  d = 5;
  listPrepend(l, &d);
  assert(listGetLength(l) == 1);
  assert(listGetItem(l, &d, 0) == sizeof(d));
  assert(d == 5);
  // try getting a non existent item
  assert(listGetItem(l, &d, 10) == -1);
  listDelete(l);

  printGreen("Passed.");

  // test list removal
  printYellow("Testing list removal...");

  // test unshift
  l = listCreate();
  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    // remove first item and check if the value is correct
    assert(listUnshift(l, &d) != -1);
    assert(listGetLength(l) == LIST_TEST_SIZE - i - 1);
    assert(d == i);
  }
  listDelete(l);

  // test POP
  l = listCreate();
  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    // remove last item and check if the value is correct
    assert(listPop(l, &d) != -1);
    assert(listGetLength(l) == LIST_TEST_SIZE - i - 1);
    assert(d == LIST_TEST_SIZE - i - 1);
  }
  listDelete(l);

  // test REMOVE
  l = listCreate();
  quickPopulate(l);
  // remove and item
  assert(listRemoveItem(l, &d, LIST_TEST_SIZE / 2) != -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 1);
  d = 10;
  // remove some items by value
  assert(listFindAndRemoveItems(l, &d, -1) != 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 2);
  d = LIST_TEST_SIZE * 2;
  // remove some items by value
  assert(listFindAndRemoveItems(l, &d, 1) == 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 2);
  assert(listRemoveItem(l, NULL, 0) != 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 3);
  d = 1;
  // remove some items by value
  assert(listRemoveItemByValue(l, &d) != -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 4);
  // remove some non existent items
  assert(listRemoveItem(l, NULL, -1) == -1);
  d = -1;
  assert(listRemoveItemByValue(l, &d) == -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 4);
  listDelete(l);

  printGreen("Passed.");

  printSuccess("\nALL TESTS PASSED\n");
}