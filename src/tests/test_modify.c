#include "list.h"
#include "test_helpers.h"
#include "unity.h"
#include <stdlib.h>

void test_replace(void) {
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
  TEST_ASSERT_EQUAL_INT(sizeof(data_1),
                        listReplaceItem(l, &data_1, LIST_TEST_SIZE - 1));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetLastItem(l, &data_2));
  TEST_ASSERT_EQUAL_INT(TEST_VALUE_2, data_2.i);

  // replace item in the middle
  data_1.i = TEST_VALUE_3;
  TEST_ASSERT_EQUAL_INT(sizeof(data_1),
                        listReplaceItem(l, &data_1, LIST_TEST_SIZE / 2));
  TEST_ASSERT_EQUAL_INT(sizeof(data_2),
                        listGetItem(l, &data_2, LIST_TEST_SIZE / 2));
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

void test_replace_by_value(void) {
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

void test_replace_by_value_string(void) {
  List *l = listCreate(STRING);
  listSetDestructor(l, free);

  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data d;
    d.s = malloc(20);
    sprintf(d.s, "string %d", i);
    listPush(l, &d);
  }

  const char *old_value = "string 5";

  union Data old_data;
  old_data.s = (char *)old_value;
  union Data new_data;
  new_data.s = malloc(20);
  sprintf(new_data.s, "new string");

  // replace
  TEST_ASSERT_NOT_EQUAL(-1, listReplaceItemByValue(l, &old_data, &new_data));
  // check if replaced
  TEST_ASSERT_EQUAL_INT(0, listCountItem(l, &old_data));
  TEST_ASSERT_EQUAL_INT(1, listCountItem(l, &new_data));

  listDelete(l);
}

void test_replace_by_value_not_found(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data missing_data, new_data;
  missing_data.i = LIST_TEST_SIZE + 1;
  new_data.i = -1;

  TEST_ASSERT_EQUAL_INT(-1, listReplaceItemByValue(l, &missing_data, &new_data));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE, listGetSize(l));

  listDelete(l);
}

void test_remove_by_value(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data old_data;
  old_data.i = LIST_TEST_SIZE / 2;

  // remove
  TEST_ASSERT_NOT_EQUAL(-1, listRemoveItemByValue(l, &old_data));
  // check if removed
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, listGetSize(l));
  TEST_ASSERT_EQUAL_INT(0, listCountItem(l, &old_data));

  // removing a value that does not exist fails
  TEST_ASSERT_EQUAL_INT(-1, listRemoveItemByValue(l, &old_data));

  listDelete(l);
}

void test_count_remove(void) {
  List *l = listCreate(INTEGER);
  for (int i = 0; i < 10; i++)
    listPush(l, &(union Data){.i = 0});

  union Data old_data;
  old_data.i = 0;

  // remove only the first 3 occurrences
  TEST_ASSERT_EQUAL_INT(3, listCountRemove(l, &old_data, 3));
  TEST_ASSERT_EQUAL_INT(7, listGetSize(l));
  TEST_ASSERT_EQUAL_INT(7, listCountItem(l, &old_data));

  // remove all remaining occurrences
  TEST_ASSERT_EQUAL_INT(7, listCountRemove(l, &old_data, -1));
  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));

  // nothing left to remove
  TEST_ASSERT_EQUAL_INT(0, listCountRemove(l, &old_data, -1));

  listDelete(l);
}

void test_count_replace(void) {
  List *l = listCreate(INTEGER);
  for (int i = 0; i < 10; i++)
    listPush(l, &(union Data){.i = 0});

  union Data old_data, new_data;
  old_data.i = 0;
  new_data.i = 1;

  // replace only the first 3 occurrences
  TEST_ASSERT_EQUAL_INT(3, listCountReplace(l, &old_data, &new_data, 3));
  TEST_ASSERT_EQUAL_INT(3, listCountItem(l, &new_data));
  TEST_ASSERT_EQUAL_INT(7, listCountItem(l, &old_data));

  // replace all remaining occurrences
  TEST_ASSERT_EQUAL_INT(7, listCountReplace(l, &old_data, &new_data, -1));
  TEST_ASSERT_EQUAL_INT(10, listCountItem(l, &new_data));
  TEST_ASSERT_EQUAL_INT(0, listCountItem(l, &old_data));

  // nothing left to replace
  TEST_ASSERT_EQUAL_INT(0, listCountReplace(l, &old_data, &new_data, -1));

  listDelete(l);
}

void test_add_item(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data data;

  // add at index 0 (head)
  data.i = -1;
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE + 1, listAddItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(-1, data.i);

  // add past the end (tail)
  data.i = -2;
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE + 2,
                        listAddItem(l, &data, LIST_TEST_SIZE + 100));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetLastItem(l, &data));
  TEST_ASSERT_EQUAL_INT(-2, data.i);

  // add in the middle
  data.i = -3;
  int middle = listGetSize(l) / 2;
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE + 3, listAddItem(l, &data, middle));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, middle));
  TEST_ASSERT_EQUAL_INT(-3, data.i);

  listDelete(l);
}

void test_remove(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // remove first item until list is empty
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data removed;
    TEST_ASSERT_EQUAL_INT(sizeof(removed), listRemoveItem(l, &removed, 0));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));

  quickPopulate(l);
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data removed;
    TEST_ASSERT_EQUAL_INT(sizeof(removed),
                          listRemoveItem(l, &removed, LIST_TEST_SIZE - i - 1));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }

  listDelete(l);
}

static int destructor_calls = 0;
static void counting_destructor(void *p) {
  destructor_calls++;
  free(p);
}

void test_destructor(void) {
  List *l = listCreate(POINTER);
  listSetDestructor(l, counting_destructor);

  // listRemoveItem and listReplaceItem should invoke the destructor
  for (int i = 0; i < 10; i++) {
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

void test_pop_push(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  // pop first item until list is empty
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data popped;
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listPop(l, &popped));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(0, listGetSize(l));

  // push items
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data pushed;
    pushed.i = i;
    TEST_ASSERT_EQUAL_INT(i + 1, listPush(l, &pushed));
    TEST_ASSERT_EQUAL_INT(i + 1, listGetSize(l));
  }
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE, listGetSize(l));

  // shift items
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data shifted;
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listShift(l, &shifted));
    TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - i - 1, listGetSize(l));
  }

  listDelete(l);
}

void test_swap(void) {
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

  // swapping with an out-of-bounds index fails and leaves the list untouched
  TEST_ASSERT_EQUAL_INT(-1, listSwap(l, 0, 100));
  TEST_ASSERT_EQUAL_INT(-1, listSwap(l, 100, 0));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 1));
  TEST_ASSERT_EQUAL_INT(0, data.i);

  // swapping with a negative index fails and leaves the list untouched
  TEST_ASSERT_EQUAL_INT(-1, listSwap(l, -1, 0));
  TEST_ASSERT_EQUAL_INT(-1, listSwap(l, 0, -1));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 1));
  TEST_ASSERT_EQUAL_INT(0, data.i);

  // swapping the index with itself is a no-op
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, 0, 0));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(1, data.i);

  // swapping with the last valid index (length - 1) succeeds
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, 0, 1));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 0));
  TEST_ASSERT_EQUAL_INT(0, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 1));
  TEST_ASSERT_EQUAL_INT(1, data.i);

  listDelete(l);
}

void test_swap_large_list(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  union Data data;
  int last = LIST_TEST_SIZE - 1;

  // both indexes close to the head: cheap to reach n2 by walking forward
  // from n1
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, 1, 3));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 1));
  TEST_ASSERT_EQUAL_INT(3, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 3));
  TEST_ASSERT_EQUAL_INT(1, data.i);

  // one index near the head and one near the tail: walking forward from n1
  // would be O(n), so n2 should be reached from the tail instead
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, 5, last - 1));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, 5));
  TEST_ASSERT_EQUAL_INT(last - 1, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, last - 1));
  TEST_ASSERT_EQUAL_INT(5, data.i);

  // both indexes close to the tail
  TEST_ASSERT_EQUAL_INT(0, listSwap(l, last - 3, last));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, last - 3));
  TEST_ASSERT_EQUAL_INT(last, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetItem(l, &data, last));
  TEST_ASSERT_EQUAL_INT(last - 3, data.i);

  listDelete(l);
}
