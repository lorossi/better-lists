#include "list.h"
#include "test_helpers.h"
#include "unity.h"
#include <stdlib.h>

void test_empty_list(void) {
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

void test_populate_list(void) {
  List *l = listCreate(INTEGER);
  union Data data_1;

  for (int i = 0; i < LIST_TEST_SIZE; i++) {
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
  data_2.i = LIST_TEST_SIZE - 1;
  // get items from list
  TEST_ASSERT_NOT_EQUAL(0, listGetItem(l, &data_1, 0));
  TEST_ASSERT_EQUAL_INT(0, data_1.i);
  TEST_ASSERT_NOT_EQUAL(0, listGetFirstItem(l, &data_1));
  TEST_ASSERT_EQUAL_INT(0, data_1.i);
  TEST_ASSERT_NOT_EQUAL(0, listGetLastItem(l, &data_1));
  TEST_ASSERT_EQUAL_INT(LIST_TEST_SIZE - 1, data_1.i);
  TEST_ASSERT_NOT_EQUAL(-1, dataInList(l, &data_2));
  // check if item is is list
  data_2.i = LIST_TEST_SIZE + 1;
  TEST_ASSERT_EQUAL_INT(-1, dataInList(l, &data_2));
  data_2.i = 0;
  TEST_ASSERT_NOT_EQUAL(-1, dataInList(l, &data_2));

  // getting a node that does not exist fails, whether the index is too
  // large or negative
  TEST_ASSERT_EQUAL_INT(-1, listGetItem(l, &data_2, LIST_TEST_SIZE));
  TEST_ASSERT_EQUAL_INT(-1, listGetItem(l, &data_2, LIST_TEST_SIZE + 100));
  TEST_ASSERT_EQUAL_INT(-1, listGetItem(l, &data_2, -1));

  listDelete(l);
}

static void populate_type(list_type type) {
  List *l = listCreate(type);
  if (type == STRING)
    listSetDestructor(l, free);

  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data data_1;
    switch (type) {
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
    case STRING:
      data_1.s = malloc(20);
      sprintf(data_1.s, "string %d", i);
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
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    TEST_ASSERT_EQUAL_INT(sizeof(data_2), listGetItem(l, &data_2, i));
    switch (type) {
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
    case STRING:
      char buffer[20];
      sprintf(buffer, "string %d", i);
      TEST_ASSERT_EQUAL_STRING(buffer, data_2.s);
      break;
    default:
      TEST_FAIL_MESSAGE("Invalid type");
      return;
    }
  }

  listDelete(l);
}

// listPush/listPrepend each have 3 branches: empty list (no head), single
// item (head but no tail), and the general case. test_append_prepend only
// ever hits the general case since it starts from a prepopulated list.
void test_push_prepend_empty_and_single(void) {
  List *l = listCreate(INTEGER);

  // push into an empty list (no head). With a single item, the head doubles
  // as the only item and the tail is still NULL, so listGetLastItem fails.
  TEST_ASSERT_EQUAL_INT(1, listPush(l, &(union Data){.i = 1}));
  union Data data;
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(-1, listGetLastItem(l, &data));

  // push into a single-item list (head but no tail)
  TEST_ASSERT_EQUAL_INT(2, listPush(l, &(union Data){.i = 2}));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetLastItem(l, &data));
  TEST_ASSERT_EQUAL_INT(2, data.i);

  listDelete(l);

  l = listCreate(INTEGER);

  // prepend into an empty list (no head). Same single-item caveat as above.
  TEST_ASSERT_EQUAL_INT(1, listPrepend(l, &(union Data){.i = 1}));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(1, data.i);
  TEST_ASSERT_EQUAL_INT(-1, listGetLastItem(l, &data));

  // prepend into a single-item list (head but no tail)
  TEST_ASSERT_EQUAL_INT(2, listPrepend(l, &(union Data){.i = 2}));
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetFirstItem(l, &data));
  TEST_ASSERT_EQUAL_INT(2, data.i);
  TEST_ASSERT_EQUAL_INT(sizeof(data), listGetLastItem(l, &data));
  TEST_ASSERT_EQUAL_INT(1, data.i);

  listDelete(l);
}

void test_populate_type_integer(void) { populate_type(INTEGER); }
void test_populate_type_float(void) { populate_type(FLOAT); }
void test_populate_type_char(void) { populate_type(CHAR); }
void test_populate_type_double(void) { populate_type(DOUBLE); }
void test_populate_type_string(void) { populate_type(STRING); }

void test_append_prepend(void) {
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
