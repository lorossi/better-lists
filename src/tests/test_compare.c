#include "list.h"
#include "test_helpers.h"
#include "unity.h"
#include <stdlib.h>

// exercises every branch of the static _nodeValueCompare via dataInList,
// for both a "found" and "not found" value of each list type
static void compare_type(list_type type) {
  List *l = listCreate(type);
  if (type == STRING)
    listSetDestructor(l, free);

  union Data present, absent;
  switch (type) {
  case INTEGER:
    present.i = 5;
    absent.i = -1;
    break;
  case FLOAT:
    present.f = 5.0f;
    absent.f = -1.0f;
    break;
  case CHAR:
    present.c = 'e';
    absent.c = 'Z';
    break;
  case DOUBLE:
    present.d = 5.0;
    absent.d = -1.0;
    break;
  case STRING:
    present.s = "string 5";
    absent.s = "missing";
    break;
  case POINTER:
    present.p = (void *)(uintptr_t)0xdeadbeef;
    absent.p = (void *)(uintptr_t)0xbadc0de;
    break;
  default:
    TEST_FAIL_MESSAGE("Invalid type");
    return;
  }

  for (int i = 0; i < 10; i++) {
    union Data data;
    switch (type) {
    case INTEGER:
      data.i = i;
      break;
    case FLOAT:
      data.f = (float)i;
      break;
    case CHAR:
      data.c = (char)('a' + i);
      break;
    case DOUBLE:
      data.d = (double)i;
      break;
    case STRING:
      data.s = malloc(20);
      sprintf(data.s, "string %d", i);
      break;
    case POINTER:
      data.p = (char *)0xdeadbeef + i;
      break;
    default:
      TEST_FAIL_MESSAGE("Invalid type");
      return;
    }
    listPush(l, &data);
  }

  TEST_ASSERT_NOT_EQUAL(-1, dataInList(l, &present));
  TEST_ASSERT_EQUAL_INT(-1, dataInList(l, &absent));

  listDelete(l);
}

void test_compare_type_integer(void) { compare_type(INTEGER); }
void test_compare_type_float(void) { compare_type(FLOAT); }
void test_compare_type_char(void) { compare_type(CHAR); }
void test_compare_type_double(void) { compare_type(DOUBLE); }
void test_compare_type_string(void) { compare_type(STRING); }
void test_compare_type_pointer(void) { compare_type(POINTER); }

// compares the pointed-to ints rather than the raw addresses
static int compare_pointed_int(void *p1, void *p2) {
  return *(int *)p1 - *(int *)p2 > 0 ? 1 : *(int *)p1 - *(int *)p2 < 0 ? -1 : 0;
}

void test_pointer_custom_comparator(void) {
  List *l = listCreate(POINTER);
  listSetDestructor(l, free);
  listSetComparator(l, compare_pointed_int);

  for (int i = 0; i < 10; i++) {
    union Data data;
    data.p = malloc(sizeof(int));
    *(int *)data.p = i;
    listPush(l, &data);
  }

  // two distinct allocations holding the same value are "equal" under the
  // custom comparator, even though their addresses differ
  int needle_value = 5;
  union Data needle;
  needle.p = &needle_value;
  TEST_ASSERT_EQUAL_INT(5, dataInList(l, &needle));

  needle_value = -1;
  TEST_ASSERT_EQUAL_INT(-1, dataInList(l, &needle));

  listDelete(l);
}

// _nodeGetData/_nodeSetData return -1 without touching the node when given a
// NULL destination/source; exercised directly through the iterator, which
// forwards its arguments without an extra NULL check of its own
void test_node_get_set_data_null(void) {
  List *l = listCreate(INTEGER);
  quickPopulate(l);

  Iterator *it = iteratorCreate(l, 0);

  TEST_ASSERT_EQUAL_INT(-1, iteratorGetData(it, NULL));
  TEST_ASSERT_EQUAL_INT(-1, iteratorSetData(it, NULL));

  // value is unchanged after the no-op NULL set
  union Data data;
  TEST_ASSERT_EQUAL_INT(sizeof(data), iteratorGetData(it, &data));
  TEST_ASSERT_EQUAL_INT(0, data.i);

  iteratorDelete(it);
  listDelete(l);
}
