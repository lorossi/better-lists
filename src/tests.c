#include <assert.h> // for assertions
#include <time.h>   // for random initialization
#include <stdlib.h> // for random values
#include "list.h"

// tested against memory leaks using VALGRIND
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

const int LIST_TEST_SIZE = 10;

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
    union Data d;
    d.i = i;
    listPush(l, &d);
  }
}

void quickPopulateReverse(List *l)
{
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data d;
    d.i = LIST_TEST_SIZE - i - 1;
    listPush(l, &d);
  }
}

void test_empty_list()
{
  List *l = listCreate(INTEGER);
  union Data d;

  assert(listGetSize(l) == 0);
  assert(listGetFirstItem(l, &d) == -1);
  assert(listGetLastItem(l, &d) == -1);
  assert(listGetItem(l, &d, 10) == -1);
  // now try to remove items
  assert(listRemoveItem(l, &d, 10) == -1);
  assert(listCountRemove(l, &d, 10) == 0);
  assert(listPop(l, &d) == -1);
  assert(listShift(l, &d) == -1);
  listDelete(l);
}

void test_populate_list()
{
  List *l = listCreate(INTEGER);
  union Data data_1;

  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    data_1.i = i;
    // append to list, check size
    assert(listPush(l, &data_1) == i + 1);
    assert(listGetSize(l) == i + 1);
    // check if item was correctly added
    union Data data_2;
    assert(listGetItem(l, &data_2, i) == sizeof(data_2));
    assert(data_2.i == i);
  }
  union Data data_2;
  // get items from list
  assert(listGetItem(l, &data_1, 0) != 0);
  assert(data_1.i == 0);
  assert(listGetFirstItem(l, &data_1) != 0);
  assert(data_1.i == 0);
  assert(listGetLastItem(l, &data_1) != 0);
  assert(data_1.i == LIST_TEST_SIZE - 1);
  assert(dataInList(l, &data_2) != 0);
  // check if item is is list
  data_2.i = LIST_TEST_SIZE + 1;
  assert(dataInList(l, &data_2) == -1);
  data_2.i = 0;
  assert(dataInList(l, &data_2) != -1);

  listDelete(l);
}

void test_populate_type(list_type type)
{
  List *l = listCreate(type);

  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data data_1;
    switch (type)
    {
    case INTEGER:
      data_1.i = i;
      break;
    case FLOAT:
      data_1.f = (float)i;
      break;
    case CHAR:
      data_1.c = (char)i;
      break;
    case DOUBLE:
      data_1.d = (double)i;
      break;
    default:
      printRed("Invalid type");
      exit(1);
    }

    // append to list, check size
    assert(listPush(l, &data_1) == i + 1);
    assert(listGetSize(l) == i + 1);
  }

  // check if item was correctly added
  union Data data_2;
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    assert(listGetItem(l, &data_2, i) == sizeof(data_2));
    switch (type)
    {
    case INTEGER:
      assert(data_2.i == i);
      break;
    case FLOAT:
      assert(data_2.f == (float)i);
      break;
    case CHAR:
      assert(data_2.c == (char)i);
      break;
    case DOUBLE:
      assert(data_2.d == (double)i);
      break;
    default:
      printRed("Invalid type");
      exit(1);
    }
  }

  listDelete(l);
}

void test_append_prepend()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  const int TEST_VALUE_1 = -1;
  const int TEST_VALUE_2 = -5;

  // replace first item
  union Data data_1, data_2;
  data_1.i = TEST_VALUE_1;
  assert(listPrepend(l, &data_1) == LIST_TEST_SIZE + 1);
  assert(listGetFirstItem(l, &data_2) == sizeof(data_2));
  assert(data_2.i == TEST_VALUE_1);
  assert(listCountItem(l, &data_2) == 1);

  // replace last item
  data_1.i = TEST_VALUE_2;
  assert(listPush(l, &data_1) == LIST_TEST_SIZE + 2);
  assert(listGetLastItem(l, &data_2) == sizeof(data_2));
  assert(data_2.i == TEST_VALUE_2);
  assert(listCountItem(l, &data_2) == 1);

  listDelete(l);
}

void test_replace()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  const int TEST_VALUE_1 = -1;
  const int TEST_VALUE_2 = -5;
  const int TEST_VALUE_3 = -10;

  // replace first item
  union Data data_1, data_2;
  data_1.i = TEST_VALUE_1;
  assert(listReplaceItem(l, &data_1, 0) == sizeof(data_1));
  assert(listGetFirstItem(l, &data_2) == sizeof(data_2));
  assert(data_2.i == TEST_VALUE_1);

  // replace last item
  data_1.i = TEST_VALUE_2;
  assert(listReplaceItem(l, &data_1, LIST_TEST_SIZE - 1) == sizeof(data_1));
  assert(listGetLastItem(l, &data_2) == sizeof(data_2));
  assert(data_2.i == TEST_VALUE_2);

  // replace item in the middle
  data_1.i = TEST_VALUE_3;
  assert(listReplaceItem(l, &data_1, LIST_TEST_SIZE / 2) == sizeof(data_1));
  assert(listGetItem(l, &data_2, LIST_TEST_SIZE / 2) == sizeof(data_2));
  assert(data_2.i == TEST_VALUE_3);

  // test count items
  union Data data_3;
  data_3.i = TEST_VALUE_1;
  assert(listCountItem(l, &data_3) == 1);
  data_3.i = TEST_VALUE_2;
  assert(listCountItem(l, &data_3) == 1);
  data_3.i = TEST_VALUE_3;
  assert(listCountItem(l, &data_3) == 1);

  listDelete(l);
}

void test_replace_by_value()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  const int old_value = LIST_TEST_SIZE / 2;
  const int new_value = -1;

  union Data old_data;
  old_data.i = old_value;
  union Data new_data;
  new_data.i = new_value;

  // replace
  assert(listReplaceItemByValue(l, &old_data, &new_data) != -1);
  // check if replaced
  assert(listCountItem(l, &old_data) == 0);
  assert(listCountItem(l, &new_data) == 1);

  listDelete(l);
}

void test_remove()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // remove first item until list is empty

  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data removed;
    assert(listRemoveItem(l, &removed, 0) == sizeof(removed));
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
  }
  assert(listGetSize(l) == 0);

  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data removed;
    assert(listRemoveItem(l, &removed, LIST_TEST_SIZE - i - 1) == sizeof(removed));
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
  }

  listDelete(l);
}

void test_pop_push()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // pop first item until list is empty
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data popped;
    assert(listPop(l, &popped) == LIST_TEST_SIZE - i - 1);
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
  }
  assert(listGetSize(l) == 0);

  // push items
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data pushed;
    pushed.i = i;
    assert(listPush(l, &pushed) == i + 1);
    assert(listGetSize(l) == i + 1);
  }
  assert(listGetSize(l) == LIST_TEST_SIZE);

  // shift items
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data shifted;
    assert(listShift(l, &shifted) == LIST_TEST_SIZE - i - 1);
    assert(listGetSize(l) == LIST_TEST_SIZE - i - 1);
  }

  listDelete(l);
}

void test_swap()
{
  List *l = listCreate(INTEGER);
  listPush(l, &(union Data){.i = 0});
  listPush(l, &(union Data){.i = 1});

  assert(listGetSize(l) == 2);
  assert(listSwap(l, 0, 1) == 0);
  assert(listGetSize(l) == 2);

  union Data data;
  assert(listGetItem(l, &data, 0) == sizeof(data));
  assert(data.i == 1);
  assert(listGetItem(l, &data, 1) == sizeof(data));
  assert(data.i == 0);

  listDelete(l);
}

void test_to_array()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data *array = malloc(sizeof(union Data) * LIST_TEST_SIZE);

  assert(listToArray(l, array) == LIST_TEST_SIZE);
  assert(array != NULL);

  for (int i = 0; i < LIST_TEST_SIZE; i++)
    assert(array[i].i == i);

  free(array);
  listDelete(l);
}

void test_iterator()
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  Iterator *it;
  union Data data;
  int i = 0;
  // test iterator FORWARD
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    assert(iteratorGetData(it, &data) == sizeof(data));
    assert(data.i == iteratorGetIndex(it));
    assert(data.i == i);
    i++;
  }
  assert(i == LIST_TEST_SIZE - 1);
  iteratorDelete(it);
  listDelete(l);

  // test iterator BACKWARD
  i = LIST_TEST_SIZE - 1;
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (it = iteratorCreate(l, -1); !iteratorStarted(it); iteratorPrevious(it))
  {
    assert(iteratorGetData(it, &data) == sizeof(data));
    assert(data.i == iteratorGetIndex(it));
    assert(data.i == i);
    i--;
  }
  assert(i == 0);
  iteratorDelete(it);
  listDelete(l);
}

void test_iterator_get_set()
{
  List *l = listCreate(INTEGER);
  Iterator *it;
  int i;
  quickPopulate(l);

  // test iterator set data
  i = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    union Data data;
    iteratorSetData(it, &(union Data){.i = -1});
    assert(iteratorGetData(it, &data) == sizeof(data));
    assert(data.i == -1);
    assert(iteratorGetIndex(it) == i);

    i++;
  }
  iteratorDelete(it);
  listDelete(l);

  // test random access
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data data;
    it = iteratorCreate(l, i);
    assert(iteratorGetIndex(it) == i);
    assert(iteratorGetData(it, &data) == sizeof(data));
  }
  iteratorDelete(it);
  listDelete(l);
}

int main()
{
  // variable declaration
  srand(time(NULL));

  // tests the empty list
  printYellow("Testing empty list...");
  // create list and try to get items
  test_empty_list();
  printGreen("Passed.");

  // test list population
  printYellow("Testing list population...");
  test_populate_list();
  test_populate_type(INTEGER);
  test_populate_type(FLOAT);
  test_populate_type(CHAR);
  test_populate_type(DOUBLE);
  printGreen("Passed.");

  // test list append and prepend
  printYellow("Testing list append and prepend...");
  test_append_prepend();
  printGreen("Passed.");

  // test list replace
  printYellow("Testing list replace...");
  test_replace();
  test_replace_by_value();
  printGreen("Passed.");

  // test list remove
  printYellow("Testing list remove...");
  test_remove();
  test_pop_push();
  printGreen("Passed.");

  // test list swap
  printYellow("Testing list swap...");
  test_swap();
  printGreen("Passed.");

  // test list to array
  printYellow("Testing list to array...");
  test_to_array();
  printGreen("Passed.");

  // test list iterator
  printYellow("Testing list iterator...");
  test_iterator();
  printGreen("Passed.");

  // printGreen("Passed.");

  // printYellow("Testing sorting...");

  // // test sorting
  // l = listCreate();
  // quickPopulateReverse(l);
  // assert(listSort(l, 0) != -1);
  // count = 0;
  // for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  // {
  //   assert(iteratorGetData(it, &d) != -1);
  //   assert(d == count);
  //   count++;
  // }
  // iteratorDelete(it);
  // listDelete(l);

  // // test reverse sorting
  // l = listCreate();
  // quickPopulate(l);
  // assert(listSort(l, 1) != -1);
  // count = LIST_TEST_SIZE - 1;
  // for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  // {
  //   assert(iteratorGetData(it, &d) != -1);
  //   assert(d == count);
  //   count--;
  // }
  // iteratorDelete(it);
  // listDelete(l);

  // // test random shuffling
  // l = listCreate();
  // quickPopulate(l);
  // count = 0;
  // int in_place = 0;
  // for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  // {
  //   assert(iteratorGetData(it, &d) != -1);
  //   if (d == count)
  //     in_place++;
  //   count++;
  // }
  // assert(in_place < listGetSize(l));
  // listDelete(l);

  // // test sorting
  // l = listCreate();
  // quickPopulateReverse(l);
  // assert(listSort(l, 0) != -1);
  // count = 0;
  // for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  // {
  //   assert(iteratorGetData(it, &d) != -1);
  //   assert(d == count);
  //   count++;
  // }
  // iteratorDelete(it);
  // listDelete(l);

  // printGreen("Passed.");

  // if we got here, all assert work correctly
  // YAY!
  printSuccess("ALL TESTS PASSED");
}