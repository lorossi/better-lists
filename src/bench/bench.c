#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

static const int SIZES[] = {1000, 10000, 100000};
static const int NUM_SIZES = sizeof(SIZES) / sizeof(SIZES[0]);

static double now(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void benchPush(int size) {
  List *list = listCreate(INTEGER);
  union Data data;

  double start = now();
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "push", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchPrepend(int size) {
  List *list = listCreate(INTEGER);
  union Data data;

  double start = now();
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPrepend(list, &data);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "prepend", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchGetItem(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }

  double start = now();
  for (int i = 0; i < size; i++) {
    listGetItem(list, &data, i);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "get_item", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchRemoveItem(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }

  double start = now();
  for (int i = size - 1; i >= 0; i--) {
    listRemoveItem(list, &data, i);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "remove_item", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchSort(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  srand(0xDEADBEEF);
  for (int i = 0; i < size; i++) {
    data.i = rand();
    listPush(list, &data);
  }

  double start = now();
  listSort(list, 0);
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "sort", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchIterate(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }

  double start = now();
  Iterator *it = iteratorCreate(list, 0);
  while (!iteratorEnded(it)) {
    iteratorGetData(it, &data);
    iteratorNext(it);
  }
  iteratorDelete(it);
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "iterate", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchPop(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }

  double start = now();
  for (int i = size - 1; i >= 0; i--) {
    listPop(list, &data);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "pop", size, elapsed,
         size / elapsed);
  listDelete(list);
}

static void benchRandomGet(int size) {
  List *list = listCreate(INTEGER);
  union Data data;
  srand(0xDEADBEEF);

  for (int i = 0; i < size; i++) {
    data.i = i;
    listPush(list, &data);
  }

  double start = now();
  for (int i = 0; i < size; i++) {
    int index = rand() % size;
    listGetItem(list, &data, index);
  }
  double elapsed = now() - start;

  printf("%-20s n=%-8d %10.6f s  (%.0f ops/s)\n", "random_access", size,
         elapsed, size / elapsed);
  listDelete(list);
}

int main(void) {
  for (int i = 0; i < NUM_SIZES; i++) {
    int size = SIZES[i];
    benchPush(size);
    benchPop(size);
    benchPrepend(size);
    benchGetItem(size);
    benchIterate(size);
    benchSort(size);
    benchRemoveItem(size);
    benchRandomGet(size);
    printf("\n");
  }

  return 0;
}
