#include <time.h>   // for random initialization
#include <stdlib.h> // for random values
#include "unity.h"
#include "list.h"

// tested against memory leaks using VALGRIND
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

// helper functions
void quickPopulate(List *l);
void quickPopulateReverse(List *l);

const int LIST_TEST_SIZE = 10000;

void setUp(void) {}
void tearDown(void) {}

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

void test_empty_list(void)
{
  List *l = listCreate(INTEGER);
  union Data d;

  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));
  TEST_ASSERT_EQUAL_INT(-1, listGetFirstItem(l, &d));
  TEST_ASSERT_EQUAL_INT(-1, listGetLastItem(l, &d));
  TEST_ASSERT_EQUAL_INT(-1, listGetItem(l, &d, 10));
  // now try to remove items
  TEST_ASSERT_EQUAL_INT(-1, listRemoveItem(l, &d, 10));
  TEST_ASSERT_EQUAL_INT(0, listCountRemove(l, &d, 10));
  TEST_ASSERT_EQUAL_INT(-1, listPop(l, &d));
  TEST_ASSERT_EQUAL_INT(-1, listShift(l, &d));
  listDelete(l);
}

void test_populate_list(void)
{
  List *l = listCreate(INTEGER);
  union Data data_1;

  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    data_1.i = i;
    // append to list, check size
    TEST_ASSERT_EQUAL_INT(i + 1, listPush(l, &data_1));
    TEST_ASSERT_EQUAL_INT(i + 1, listGetSize(l));
    // check if item was correctly added
    union Data data_2;
    TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetItem(l, &data_2, i));
    TEST_ASSERT_EQUAL_INT(i, data_2.i);
  }
  union Data data_2;
  // get items from list
  TEST_ASSERT_NOT_EQUAL(0, listGetItem(l, &data_1, 0));
  TEST_ASSERT_EQUAL_INT(0, data_1.i);
  TEST_ASSERT_NOT_EQUAL(0, listGetFirstItem(l, &data_1));
  TEST_ASSERT_EQUAL_INT(0, data_1.i);
  TEST_ASSERT_NOT_EQUAL(0, listGetLastItem(l, &data_1));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, data_1.i);
  TEST_ASSERT_NOT_EQUAL(0, dataInList(l, &data_2));
  // check if item is is list
  data_2.i = LIST_TEST_SIZE + 1;
  TEST_ASSERT_EQUAL_INT(-1, dataInList(l, &data_2));
  data_2.i = 0;
  TEST_ASSERT_NOT_EQUAL(-1, dataInList(l, &data_2));

  listDelete(l);
}

static void populate_type(list_type type)
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
      TEST_FAIL_MESSAGE("Invalid type");
      return;
    }

    // append to list, check size
    TEST_ASSERT_EQUAL_INT(i + 1, listPush(l, &data_1));
    TEST_ASSERT_EQUAL_INT(i + 1, listGetSize(l));
  }

  // check if item was correctly added
  union Data data_2;
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetItem(l, &data_2, i));
    switch (type)
    {
    case INTEGER:
      TEST_ASSERT_EQUAL_INT(i, data_2.i);
      break;
    case FLOAT:
      TEST_ASSERT_EQUAL_FLOAT((float)i, data_2.f);
      break;
    case CHAR:
      TEST_ASSERT_EQUAL_INT((char)i, data_2.c);
      break;
    case DOUBLE:
      TEST_ASSERT_EQUAL_DOUBLE((double)i, data_2.d);
      break;
    default:
      TEST_FAIL_MESSAGE("Invalid type");
      return;
    }
  }

  listDelete(l);
}

void test_populate_type_integer(void) { populate_type(INTEGER); }
void test_populate_type_float(void) { populate_type(FLOAT); }
void test_populate_type_char(void) { populate_type(CHAR); }
void test_populate_type_double(void) { populate_type(DOUBLE); }

void test_append_prepend(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  const int TEST_VALUE_1 = -1;
  const int TEST_VALUE_2 = -5;

  // replace first item
  union Data data_1, data_2;
  data_1.i = TEST_VALUE_1;
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE + 1, listPrepend(l, &data_1));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetFirstItem(l, &data_2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_1, data_2.i);
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &data_2));

  // replace last item
  data_1.i = TEST_VALUE_2;
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE + 2, listPush(l, &data_1));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetLastItem(l, &data_2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_2, data_2.i);
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &data_2));

  listDelete(l);
}

void test_replace(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  const int TEST_VALUE_1 = -1;
  const int TEST_VALUE_2 = -5;
  const int TEST_VALUE_3 = -10;

  // replace first item
  union Data data_1, data_2;
  data_1.i = TEST_VALUE_1;
  TEST_ASSERT_EQUAL_INT(sizeof(data_1), listReplaceItem(l, &data_1, 0));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetFirstItem(l, &data_2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_1, data_2.i);

  // replace last item
  data_1.i = TEST_VALUE_2;
  TEST_ASSERT_EQUAL_INT(sizeof(data_1), listReplaceItem(l, &data_1, LIST_TEST_SIZE - 1));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetLastItem(l, &data_2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_2, data_2.i);

  // replace item in the middle
  data_1.i = TEST_VALUE_3;
  TEST_ASSERT_EQUAL_INT(sizeof(data_1), listReplaceItem(l, &data_1, LIST_TEST_SIZE / 2));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetItem(l, &data_2, LIST_TEST_SIZE / 2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_3, data_2.i);

  // test count items
  union Data data_3;
  data_3.i = TEST_VALUE_1;
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &data_3));
  data_3.i = TEST_VALUE_2;
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &data_3));
  data_3.i = TEST_VALUE_3;
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &data_3));

  listDelete(l);
}

void test_replace_by_value(void)
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
  TEST_ASSERT_NOT_EQUAL(-1, listReplaceItemByValue(l, &old_data, &new_data));
  // check if replaced
  TEST_ASSERT_EQUAL_INT(0, listCountItem(l, &old_data));
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &new_data));

  listDelete(l);
}

void test_remove(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // remove first item until list is empty
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data removed;
    TEST_ASSERT_EQUAL_INT(sizeof(removed), listRemoveItem(l, &removed, 0));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));

  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data removed;
    TEST_ASSERT_EQUAL_INT(sizeof(removed), listRemoveItem(l, &removed, LIST_TEST_SIZE - i - 1));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }

  listDelete(l);
}

static int destructor_calls = 0;
static void counting_destructor(void *p)
{
  destructor_calls++;
  free(p);
}

void test_destructor(void)
{
  List *l = listCreate(POINTER);
  listSetDestructor(l, counting_destructor);

  // listRemoveItem and listReplaceItem should invoke the destructor
  for (int i = 0; i < 10; i++)
  {
    union Data data;
    data.p = malloc(sizeof(int));
    listPush(l, &data);
  }
  destructor_calls = 0;

  // discarding a removed item invokes the destructor
  TEST_ASSERT_NOT_EQUAL(-1, listRemoveItem(l, NULL, 0));
  TEST_ASSERT_EQUAL_INT(1, destructor_calls);

  // taking ownership via destination does not invoke the destructor
  union Data removed;
  TEST_ASSERT_NOT_EQUAL(-1, listRemoveItem(l, &removed, 0));
  TEST_ASSERT_EQUAL_INT(1, destructor_calls);
  free(removed.p);

  // replacing an item invokes the destructor on the old value
  union Data new_data;
  new_data.p = malloc(sizeof(int));
  TEST_ASSERT_NOT_EQUAL(-1, listReplaceItem(l, &new_data, 0));
  TEST_ASSERT_EQUAL_INT(2, destructor_calls);

  // deleting the list invokes the destructor on all remaining items
  int remaining = listGetSize(l);
  listDelete(l);
  TEST_ASSERT_EQUAL_INT(2 + remaining, destructor_calls);
}

void test_pop_push(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // pop first item until list is empty
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data popped;
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listPop(l, &popped));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));

  // push items
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data pushed;
    pushed.i = i;
    TEST_ASSERT_EQUAL_INT(i + 1, listPush(l, &pushed));
    TEST_ASSERT_EQUAL_INT(i + 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE, listGetSize(l));

  // shift items
  for (int i = 0; i < LIST_TEST_SIZE; i++)
  {
    union Data shifted;
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listShift(l, &shifted));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }

  listDelete(l);
}

void test_swap(void)
{
  List *l = listCreate(INTEGER);
  listPush(l, &(union Data){.i = 0});
  listPush(l, &(union Data){.i = 1});

  TEST_ASSERT_EQUAL_INT(2, listGetSize(l));
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, 0, 1));
  TEST_ASSERT_EQUAL_INT(2, listGetSize(l));

  union Data data;
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 1));
  TEST_ASSERT_EQUAL_INT(0, data.i);

  listDelete(l);
}

void test_to_array(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data *array = malloc(sizeof(union Data) * LIST_TEST_SIZE);

  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE, listToArray(l, array));
  TEST_ASSERT_NOT_NULL(array);

  for (int i = 0; i < LIST_TEST_SIZE; i++)
    TEST_ASSERT_EQUAL_INT(i, array[i].i);

  free(array);
  listDelete(l);
}

void test_iterator(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  Iterator *it;
  union Data data;
  int i = 0;
  // test iterator FORWARD
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
    TEST_ASSERT_EQUAL_INT(i, data.i);
    i++;
  }
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, i);
  iteratorDelete(it);
  listDelete(l);

  // test iterator BACKWARD
  i = LIST_TEST_SIZE - 1;
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (it = iteratorCreate(l, -1); !iteratorStarted(it); iteratorPrevious(it))
  {
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
    TEST_ASSERT_EQUAL_INT(i, data.i);
    i--;
  }
  TEST_ASSERT_EQUAL_INT(0, i);
  iteratorDelete(it);
  listDelete(l);
}

void test_iterator_get_set(void)
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
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(-1, data.i);
    TEST_ASSERT_EQUAL_INT(i, iteratorGetIndex(it));

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
    TEST_ASSERT_EQUAL_INT(i, iteratorGetIndex(it));
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    iteratorDelete(it);
  }
  listDelete(l);
}

void test_sort(void)
{
  List *l = listCreate(INTEGER);
  quickPopulateReverse(l);
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 0));

  Iterator *it;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);
  listDelete(l);
}

void test_sort_reverse(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 1));

  Iterator *it;
  for (it = iteratorCreate(l, -1); !iteratorEnded(it); iteratorNext(it))
  {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);
  listDelete(l);
}

void test_shuffle(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  TEST_ASSERT_EQUAL_INT(0, listShuffle(l));

  Iterator *it;
  int count = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    union Data data;
    iteratorGetData(it, &data);
    if (data.i != iteratorGetIndex(it))
      count++;
  }
  iteratorDelete(it);
  TEST_ASSERT_GREATER_THAN(0, count);

  // now sort the list back
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 0));
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it))
  {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);

  listDelete(l);
}

void test_helper_get(void)
{
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  TEST_ASSERT_EQUAL_INT(0, listGetInt(l, 0));
  listDelete(l);

  l = listCreate(FLOAT);
  listPush(l, &(union Data){.f = 1.0});
  TEST_ASSERT_EQUAL_FLOAT(1.0, listGetFloat(l, 0));
  listDelete(l);

  l = listCreate(DOUBLE);
  listPush(l, &(union Data){.d = 1.0});
  TEST_ASSERT_EQUAL_DOUBLE(1.0, listGetDouble(l, 0));
  listDelete(l);

  l = listCreate(CHAR);
  listPush(l, &(union Data){.c = 'a'});
  TEST_ASSERT_EQUAL_INT('a', listGetChar(l, 0));
  listDelete(l);

  l = listCreate(STRING);
  listPush(l, &(union Data){.s = "test"});
  TEST_ASSERT_EQUAL_STRING("test", listGetString(l, 0));
  listDelete(l);

  l = listCreate(POINTER);
  listPush(l, &(union Data){.p = (void *)0xdeadbeef});
  TEST_ASSERT_EQUAL_PTR((void *)0xdeadbeef, listGetPointer(l, 0));
  listDelete(l);
}

int main(void)
{
  srand(time(NULL));

  UNITY_BEGIN();

  RUN_TEST(test_empty_list);

  RUN_TEST(test_populate_list);
  RUN_TEST(test_populate_type_integer);
  RUN_TEST(test_populate_type_float);
  RUN_TEST(test_populate_type_char);
  RUN_TEST(test_populate_type_double);

  RUN_TEST(test_append_prepend);

  RUN_TEST(test_replace);
  RUN_TEST(test_replace_by_value);

  RUN_TEST(test_remove);
  RUN_TEST(test_pop_push);

  RUN_TEST(test_destructor);

  RUN_TEST(test_swap);

  RUN_TEST(test_to_array);

  RUN_TEST(test_iterator);
  RUN_TEST(test_iterator_get_set);

  RUN_TEST(test_sort);
  RUN_TEST(test_sort_reverse);
  RUN_TEST(test_shuffle);

  RUN_TEST(test_helper_get);

  return UNITY_END();
}
