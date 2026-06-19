#include "test_helpers.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int LIST_TEST_SIZE = 10000;
const int SEED = 0xDEADBEEF;

static int saved_stdout_fd = -1;
static FILE *capture_file = NULL;

void captureStdoutStart(void) {
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1, saved_stdout_fd,
                               "stdout capture already active");
  TEST_ASSERT_NULL_MESSAGE(capture_file, "stdout capture already active");

  fflush(stdout);
  capture_file = tmpfile();
  TEST_ASSERT_NOT_NULL_MESSAGE(capture_file, "failed to create temp file");

  saved_stdout_fd = dup(fileno(stdout));
  TEST_ASSERT_NOT_EQUAL_MESSAGE(-1, saved_stdout_fd, "failed to dup stdout");

  TEST_ASSERT_NOT_EQUAL_MESSAGE(
      -1, dup2(fileno(capture_file), fileno(stdout)),
      "failed to redirect stdout");
}

char *captureStdoutEnd(void) {
  TEST_ASSERT_NOT_NULL_MESSAGE(capture_file, "stdout capture not active");
  TEST_ASSERT_NOT_EQUAL_MESSAGE(-1, saved_stdout_fd, "stdout capture not active");

  fflush(stdout);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(-1, dup2(saved_stdout_fd, fileno(stdout)),
                               "failed to restore stdout");
  TEST_ASSERT_NOT_EQUAL_MESSAGE(-1, close(saved_stdout_fd),
                               "failed to close saved stdout fd");
  saved_stdout_fd = -1;

  long size = ftell(capture_file);
  rewind(capture_file);

  char *buffer = malloc(size + 1);
  size_t read_bytes = fread(buffer, 1, size, capture_file);
  buffer[read_bytes] = '\0';

  fclose(capture_file);
  capture_file = NULL;

  return buffer;
}

void quickPopulate(List *l) {
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data d;
    d.i = i;
    listPush(l, &d);
  }
}

void quickPopulateReverse(List *l) {
  for (int i = 0; i < LIST_TEST_SIZE; i++) {
    union Data d;
    d.i = LIST_TEST_SIZE - i - 1;
    listPush(l, &d);
  }
}
