#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "list.h"

extern const int LIST_TEST_SIZE;
extern const int SEED;

void quickPopulate(List *l);
void quickPopulateReverse(List *l);

// Redirects stdout to a temporary file; call captureStdoutEnd to restore stdout
// and retrieve everything written since the matching captureStdoutStart.
// Only one capture may be active at a time. The returned buffer is owned by
// the caller and must be freed.
void captureStdoutStart(void);
char *captureStdoutEnd(void);

#endif
