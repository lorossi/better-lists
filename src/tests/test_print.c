#include "list.h"
#include "test_helpers.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

void test_print_list_empty(void) {
  List *l = listCreate(INTEGER);

  captureStdoutStart();
  printList(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("", output);

  free(output);
  listDelete(l);
}

void test_print_list_integer(void) {
  List *l = listCreate(INTEGER);
  for (int i = 0; i < 5; i++)
    listPush(l, &(union Data){.i = i});

  captureStdoutStart();
  printList(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("0\n1\n2\n3\n4\n", output);

  free(output);
  listDelete(l);
}

void test_print_list_custom_separator(void) {
  List *l = listCreate(INTEGER);
  for (int i = 0; i < 3; i++)
    listPush(l, &(union Data){.i = i});

  captureStdoutStart();
  printList(l, ", ");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("0, 1, 2, ", output);

  free(output);
  listDelete(l);
}

void test_print_list_string(void) {
  List *l = listCreate(STRING);
  listPush(l, &(union Data){.s = "foo"});
  listPush(l, &(union Data){.s = "bar"});

  captureStdoutStart();
  printList(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("foo\nbar\n", output);

  free(output);
  listDelete(l);
}

void test_print_list_float_double(void) {
  List *l = listCreate(FLOAT);
  listPush(l, &(union Data){.f = 1.5f});

  char expected[64];
  snprintf(expected, sizeof(expected), "%f\n", 1.5f);

  captureStdoutStart();
  printList(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING(expected, output);

  free(output);
  listDelete(l);

  l = listCreate(DOUBLE);
  listPush(l, &(union Data){.d = 2.5});

  snprintf(expected, sizeof(expected), "%lf\n", 2.5);

  captureStdoutStart();
  printList(l, "\n");
  output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING(expected, output);

  free(output);
  listDelete(l);
}

void test_print_list_pointer(void) {
  List *l = listCreate(POINTER);
  void *ptr = (void *)(uintptr_t)0xdeadbeef;
  listPush(l, &(union Data){.p = ptr});

  char expected[64];
  snprintf(expected, sizeof(expected), "%p\n", ptr);

  captureStdoutStart();
  printList(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING(expected, output);

  free(output);
  listDelete(l);
}

void test_print_list_reverse(void) {
  List *l = listCreate(INTEGER);
  for (int i = 0; i < 5; i++)
    listPush(l, &(union Data){.i = i});

  captureStdoutStart();
  printListReverse(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("4\n3\n2\n1\n0\n", output);

  free(output);
  listDelete(l);
}

void test_print_list_reverse_empty(void) {
  List *l = listCreate(INTEGER);

  captureStdoutStart();
  printListReverse(l, "\n");
  char *output = captureStdoutEnd();

  TEST_ASSERT_EQUAL_STRING("", output);

  free(output);
  listDelete(l);
}
