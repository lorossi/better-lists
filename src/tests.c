#include <assert.h>
#include "list.h"

const int LIST_TEST_SIZE = 5000;

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
  l = listCreate();
  assert(listGetLength(l) == 0);
  assert(listGetFirstItem(l, &d) == -1);
  assert(listGetLastItem(l, &d) == -1);
  assert(listGetItem(l, &d, 10) == -1);
  assert(listRemoveItem(l, &d, 10) == -1);
  assert(listFindAndRemoveItems(l, &d, 10) == 0);
  assert(listPop(l, &d) == -1);
  assert(listUnshift(l, &d) == -1);
  listDelete(l);
  assert(listGetLength(l) == 0);
  assert(listGetFirstItem(l, &d) == -1);
  printGreen("Passed.");

  // test list population TODO
  printYellow("Testing list population...");
  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    d = i;
    assert(listAppend(l, &d) == i);
    assert(listGetLength(l) == i + 1);
    assert(listGetItem(l, &t, i) == sizeof(t));
    assert(t == i);
  }
  assert(listGetItem(l, NULL, 0) != 0);
  assert(listGetFirstItem(l, NULL) != 0);
  assert(listGetLastItem(l, NULL) != 0);
  assert(itemInList(l, &d) != 0);
  t = LIST_TEST_SIZE + 1;
  assert(itemInList(l, &t) == -1);
  listDelete(l);
  assert(itemInList(l, &d) == -1);
  l = listCreate();
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    Data d = i;
    assert(listPrepend(l, &d) == 0);
    assert(listGetLength(l) == i + 1);
    Data t;
    assert(listGetItem(l, &t, 0) == sizeof(t));
    assert(t == i);
  }
  d = 100;
  assert(listReplaceItem(l, &d, 0) != 0);
  assert(listGetFirstItem(l, &t) != 0);
  assert(d == t);
  t = 0;
  assert(listReplaceItemByValue(l, &d, &t) != 0);
  listDelete(l);

  l = listCreate();
  d = 5;
  listPrepend(l, &d);
  assert(listGetLength(l) == 1);
  assert(listGetItem(l, &d, 0) == sizeof(d));
  assert(d == 5);
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
    assert(listPop(l, &d) != -1);
    assert(listGetLength(l) == LIST_TEST_SIZE - i - 1);
    assert(d == LIST_TEST_SIZE - i - 1);
  }
  listDelete(l);

  // test REMOVE
  l = listCreate();
  quickPopulate(l);
  assert(listRemoveItem(l, &d, LIST_TEST_SIZE / 2) != -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 1);
  d = 10;
  assert(listFindAndRemoveItems(l, &d, -1) != 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 2);
  d = LIST_TEST_SIZE * 2;
  assert(listFindAndRemoveItems(l, &d, 1) == 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 2);
  assert(listRemoveItem(l, NULL, 0) != 0);
  assert(listGetLength(l) == LIST_TEST_SIZE - 3);
  d = 1;
  assert(listRemoveItemByValue(l, &d) != -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 4);

  assert(listRemoveItem(l, NULL, -1) == -1);
  d = -1;
  assert(listRemoveItemByValue(l, &d) == -1);
  assert(listGetLength(l) == LIST_TEST_SIZE - 4);

  printGreen("Passed.");

  printSuccess("\nALL TESTS PASSED\n");
}