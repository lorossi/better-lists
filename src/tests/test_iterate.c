#include "list.h"
#include "test_helpers.h"
#include "unity.h"
#include <stdlib.h>

void test_to_array(void) {
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

void test_iterator(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  Iterator *it;
  union Data data;
  int i = 0;
  // test iterator FORWARD
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
    TEST_ASSERT_EQUAL_INT(i, data.i);
    i++;
  }
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE, i);
  iteratorDelete(it);
  listDelete(l);

  // test iterator BACKWARD
  i = LIST_TEST_SIZE - 1;
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (it = iteratorCreate(l, -1); !iteratorStarted(it); iteratorPrevious(it)) {
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
    TEST_ASSERT_EQUAL_INT(i, data.i);
    i--;
  }
  TEST_ASSERT_EQUAL_INT(-1, i);
  iteratorDelete(it);
  listDelete(l);
}

void test_iterator_get_set(void) {
  List *l = listCreate(INTEGER);
  Iterator *it;
  int i;
  quickPopulate(l);

  // test iterator set data
  i = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    union Data data;
    iteratorSetData(it, &(union Data){.i = -1});
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    TEST_ASSERT_EQUAL_INT(-1, data.i);
    TEST_ASSERT_EQUAL_INT(i, iteratorGetIndex(it));

    i++;
  }
  iteratorDelete(it);
  listDelete(l);

  // test iterator get node
  l = listCreate(INTEGER);
  quickPopulate(l);
  Node node;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    TEST_ASSERT_EQUAL_INT(sizeof(node), iteratorGetNode(it, &node));
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), node.data.i);
  }
  iteratorDelete(it);
  listDelete(l);

  // test get that does not exist
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    TEST_ASSERT_EQUAL_INT(-1, iteratorGetNode(it, NULL));
    TEST_ASSERT_EQUAL_INT(-1, iteratorGetData(it, NULL));
  }
  iteratorDelete(it);
  listDelete(l);

  // test random access
  l = listCreate(INTEGER);
  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data data;
    it = iteratorCreate(l, i);
    TEST_ASSERT_EQUAL_INT(i, iteratorGetIndex(it));
    TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
    iteratorDelete(it);
  }
  listDelete(l);
}

void test_iterator_edge_cases(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // the iterator is not "ended" while still sitting on the last item;
  // iteratorEnded only becomes true once iteratorNext steps past it
  Iterator *it = iteratorCreate(l, -1);
  TEST_ASSERT_EQUAL_INT(0, iteratorEnded(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorNext(it));
  TEST_ASSERT_EQUAL_INT(1, iteratorEnded(it));
  // stepping past the end fails
  TEST_ASSERT_EQUAL_INT(-1, iteratorNext(it));
  // iteratorGetNode fails on the post-end sentinel, since there is no
  // current node to copy
  Node node;
  TEST_ASSERT_EQUAL_INT(-1, iteratorGetNode(it, &node));
  // but stepping back from the post-end sentinel re-enters on the last item
  TEST_ASSERT_EQUAL_INT(0, iteratorPrevious(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorEnded(it));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, iteratorGetIndex(it));
  iteratorDelete(it);

  // symmetrically, the iterator is not "started" while sitting on the
  // first item; iteratorStarted only becomes true once iteratorPrevious
  // steps past it
  it = iteratorCreate(l, 0);
  TEST_ASSERT_EQUAL_INT(0, iteratorStarted(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorPrevious(it));
  TEST_ASSERT_EQUAL_INT(1, iteratorStarted(it));
  // stepping before the start fails
  TEST_ASSERT_EQUAL_INT(-1, iteratorPrevious(it));
  // but stepping forward from the pre-start sentinel re-enters on the
  // first item
  TEST_ASSERT_EQUAL_INT(0, iteratorNext(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorStarted(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorGetIndex(it));
  iteratorDelete(it);

  // an index past the end of the list is clamped to the last item
  it = iteratorCreate(l, LIST_TEST_SIZE + 100);
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, iteratorGetIndex(it));
  TEST_ASSERT_EQUAL_INT(0, iteratorEnded(it));
  union Data data;
  TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, data.i);
  iteratorDelete(it);

  listDelete(l);
}

void test_iterator_empty_list(void) {
  List *l = listCreate(INTEGER);

  // creating an iterator on an empty list must not crash, regardless of the
  // requested starting index
  Iterator *it = iteratorCreate(l, 0);
  TEST_ASSERT_EQUAL_INT(1, iteratorEnded(it));
  TEST_ASSERT_EQUAL_INT(1, iteratorStarted(it));
  TEST_ASSERT_EQUAL_INT(-1, iteratorNext(it));
  TEST_ASSERT_EQUAL_INT(-1, iteratorPrevious(it));
  iteratorDelete(it);

  it = iteratorCreate(l, -1);
  TEST_ASSERT_EQUAL_INT(1, iteratorEnded(it));
  TEST_ASSERT_EQUAL_INT(1, iteratorStarted(it));
  iteratorDelete(it);

  listDelete(l);
}

void test_sort_invalid_params(void) {
  List *l = listCreate(INTEGER);
  quickPopulateReverse(l);

  // reverse must be 0 or 1
  TEST_ASSERT_EQUAL_INT(-1, listSort(l, 2));
  TEST_ASSERT_EQUAL_INT(-1, listSort(l, -1));

  // the list is left untouched after a rejected sort
  union Data data;
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, data.i);

  listDelete(l);

  // a list with fewer than 2 items is trivially sorted, regardless of order
  l = listCreate(INTEGER);
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 0));
  listDelete(l);

  l = listCreate(INTEGER);
  listPush(l, &(union Data){.i = 42});
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 1));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(42, data.i);
  listDelete(l);
}

void test_sort(void) {
  List *l = listCreate(INTEGER);
  quickPopulateReverse(l);
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 0));

  Iterator *it;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);
  listDelete(l);
}

void test_sort_reverse(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 1));

  Iterator *it;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1 - iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);
  listDelete(l);
}

void test_shuffle(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);
  TEST_ASSERT_EQUAL_INT(0, listShuffle(l));

  Iterator *it;
  int count = 0;
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    union Data data;
    iteratorGetData(it, &data);
    if (data.i != iteratorGetIndex(it))
      count++;
  }
  iteratorDelete(it);
  TEST_ASSERT_GREATER_THAN(0, count);

  // now sort the list back
  TEST_ASSERT_EQUAL_INT(0, listSort(l, 0));
  for (it = iteratorCreate(l, 0); !iteratorEnded(it); iteratorNext(it)) {
    union Data data;
    iteratorGetData(it, &data);
    TEST_ASSERT_EQUAL_INT(iteratorGetIndex(it), data.i);
  }
  iteratorDelete(it);

  listDelete(l);
}

void test_helper_get(void) {
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
