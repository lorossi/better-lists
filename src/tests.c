#include <assert.h> // for assertions
#include <time.h>   // for random initialization
#include <stdlib.h> // for random values
#include "list.h"

// tested against memory leaks using VALGRIND
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

const int LIST_TEST_SIZE = 10000;

void printGreen(char *s)
{
  printf("\x1b[92;1m%s\x1b[0m\n", s);
}

void printYellow(char *s)
{
  printf("\x1b[93;1m%s\x1b[0m ", s);
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
    Data d = (Data)i;
    listAppend(l, &d);
  }
}

void quickPopulateReverse(List *l)
{
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    Data d = (Data)LIST_TEST_SIZE - i - 1;
    listAppend(l, &d);
  }
}

int main()
{
  // variable declaration
  Data d, t;
  List *l;
  Iterator *it;
  int count;
  srand(time(NULL));

  // tests the empty list
  printYellow("Testing empty list...");
  // create list and try to get items
  l = listCreate();
  assert(listGetSize(l) == 0);
  assert(listGetFirstItem(l, &d) == -1);
  assert(listGetLastItem(l, &d) == -1);
  assert(listGetItem(l, &d, 10) == -1);
  // now try to remove items
  assert(listRemoveItem(l, &d, 10) == -1);
  assert(listCountRemove(l, &d, 10) == 0);
  assert(listPop(l, &d) == -1);
  assert(listUnshift(l, &d) == -1);
  listDelete(l);
  printGreen("Passed.");

  // test list population
  printYellow("Testing list population...");
  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    d = i;
    // append to list, check size
    assert(listAppend(l, &d) == i);
    assert(listGetSize(l) == i + 1);
    // check if item was correctly added
    assert(listGetItem(l, &t, i) == sizeof(t));
    assert(t == i);
  }
  // get items from list
  assert(listGetItem(l, NULL, 0) != 0);
  assert(listGetFirstItem(l, NULL) != 0);
  assert(listGetLastItem(l, NULL) != 0);
  assert(dataInList(l, &d) != 0);
  // check if item is is list
  t = LIST_TEST_SIZE + 1;
  assert(dataInList(l, &t) == -1);
  t = 0;
  assert(dataInList(l, &t) != -1);
  listDelete(l);

  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    Data d = i;
    // append data to list
    assert(listPrepend(l, &d) == 0);
    assert(listGetSize(l) == i + 1);
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

  // try adding elements
  l = listCreate();
  quickPopulate(l);
  d = 999;
  listAddItem(l, &d, LIST_TEST_SIZE + 1);
  assert(listGetItem(l, &t, LIST_TEST_SIZE) != -1);
  assert(t == d);
  listDelete(l);

  // create list and add item in front
  l = listCreate();
  d = 5;
  listPrepend(l, &d);
  assert(listGetSize(l) == 1);
  assert(listGetItem(l, &d, 0) == sizeof(d));
  assert(d == 5);
  // try getting a non existent item
  assert(listGetItem(l, &d, 10) == -1);
  listDelete(l);

  // swap items in a list
  l = listCreate();
  for (int i = 0; i < 10; i++)
  {
    d = i;
    listAppend(l, &d);
  }

  assert(listGetFirstItem(l, &d) > 0);
  assert(d == 0);
  assert(listGetLastItem(l, &d) > 0);
  assert(d == 9);

  assert(listSwap(l, 0, 9) != -1);
  assert(listGetFirstItem(l, &d) > 0);
  assert(d == 9);
  assert(listGetLastItem(l, &d) > 0);
  assert(d == 0);

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
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
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
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
    assert(d == LIST_TEST_SIZE - i - 1);
  }
  listDelete(l);

  // test REMOVE
  l = listCreate();
  quickPopulate(l);
  // remove and item
  assert(listRemoveItem(l, &d, LIST_TEST_SIZE / 2) != -1);
  assert(listGetSize(l) == LIST_TEST_SIZE - 1);
  d = LIST_TEST_SIZE / 4;
  // remove some items by value
  assert(listCountRemove(l, &d, -1) != 0);
  assert(listGetSize(l) == LIST_TEST_SIZE - 2);
  d = LIST_TEST_SIZE * 2;
  // remove some items by value
  assert(listCountRemove(l, &d, 1) == 0);
  assert(listGetSize(l) == LIST_TEST_SIZE - 2);
  assert(listRemoveItem(l, NULL, 0) != 0);
  assert(listGetSize(l) == LIST_TEST_SIZE - 3);
  d = 1;
  // remove some items by value
  assert(listRemoveItemByValue(l, &d) != -1);
  assert(listGetSize(l) == LIST_TEST_SIZE - 4);
  // remove some non existent items
  assert(listRemoveItem(l, NULL, -1) == -1);
  d = -1;
  assert(listRemoveItemByValue(l, &d) == -1);
  assert(listGetSize(l) == LIST_TEST_SIZE - 4);
  listDelete(l);

  // test list to array
  l = listCreate();
  quickPopulate(l);
  Data a[LIST_TEST_SIZE];
  assert(listToArray(l, a) == LIST_TEST_SIZE);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
    assert(i == a[i]);
  listDelete(l);

  printGreen("Passed.");

  printYellow("Testing iterators...");
  // create iterator and move it around
  l = listCreate();
  quickPopulate(l);
  it = iteratorCreate(l, 0);
  assert(iteratorGetData(it, &d) != -1);
  assert(d == 0);
  assert(iteratorPrevious(it) == -1);
  assert(iteratorNext(it) != -1);
  assert(iteratorGetData(it, &d) != -1);
  assert(d == 1);
  assert(iteratorEnded(it) == 0);
  iteratorDelete(it);
  listDelete(l);

  // test if iterator correctly reaches end
  l = listCreate();
  quickPopulate(l);
  it = iteratorCreate(l, 0);

  for (int i = 0; i < LIST_TEST_SIZE; i++)
    iteratorNext(it);

  iteratorGetData(it, &d);
  assert(iteratorEnded(it) == 1);

  iteratorDelete(it);
  listDelete(l);

  l = listCreate();
  quickPopulate(l);
  count = 0;
  // test forward iteration
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    if (count == 0)
      assert(iteratorStarted(it));
    assert(iteratorGetData(it, &d) != -1);
    assert(d == count);
    count++;
  }

  // test backward iteration
  iteratorDelete(it);
  count = 0;
  for (it = iteratorCreate(l, -1); !iteratorStarted(it); iteratorPrevious(it))
  {
    if (count == 0)
      assert(iteratorEnded(it));
    assert(iteratorGetData(it, &d) != -1);
    assert(d == LIST_TEST_SIZE - count - 1);
    count++;
  }
  iteratorDelete(it);

  it = iteratorCreate(l, 100);
  assert(iteratorGetData(it, &d) != -1);
  assert(d = 100);
  iteratorDelete(it);
  listDelete(l);

  // test random read
  l = listCreate();
  quickPopulate(l);
  int start;

  for (int i = 0; i < 1000; i++)
  {
    start = rand() % LIST_TEST_SIZE;
    it = iteratorCreate(l, start);
    assert(iteratorGetData(it, &d) != -1);
    assert(d == start);
    iteratorDelete(it);
  }

  listDelete(l);

  printGreen("Passed.");

  printYellow("Testing sorting...");

  // test sorting
  l = listCreate();
  quickPopulateReverse(l);
  assert(listSort(l, 0) != -1);
  count = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    assert(iteratorGetData(it, &d) != -1);
    assert(d == count);
    count++;
  }
  iteratorDelete(it);
  listDelete(l);

  // test reverse sorting
  l = listCreate();
  quickPopulate(l);
  assert(listSort(l, 1) != -1);
  count = LIST_TEST_SIZE - 1;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    assert(iteratorGetData(it, &d) != -1);
    assert(d == count);
    count--;
  }
  iteratorDelete(it);
  listDelete(l);

  // test random shuffling
  l = listCreate();
  quickPopulate(l);
  count = 0;
  int in_place = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    assert(iteratorGetData(it, &d) != -1);
    if (d == count)
      in_place++;
    count++;
  }
  assert(in_place < listGetSize(l));
  listDelete(l);

  // test sorting
  l = listCreate();
  quickPopulateReverse(l);
  assert(listSort(l, 0) != -1);
  count = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    assert(iteratorGetData(it, &d) != -1);
    assert(d == count);
    count++;
  }
  iteratorDelete(it);
  listDelete(l);

  printGreen("Passed.");

  // if we got here, all assert work correctly
  // YAY!
  printSuccess("ALL TESTS PASSED");
}