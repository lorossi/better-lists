#include "test_helpers.h"
#include "unity.h"
#include <stdlib.h>

// tested against memory leaks using VALGRIND
// https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks

void setUp(void) {}
void tearDown(void) {}

void test_empty_list(void);
void test_populate_list(void);
void test_populate_type_integer(void);
void test_populate_type_float(void);
void test_populate_type_char(void);
void test_populate_type_double(void);
void test_populate_type_string(void);
void test_append_prepend(void);
void test_push_prepend_empty_and_single(void);

void test_replace(void);
void test_replace_by_value(void);
void test_replace_by_value_string(void);
void test_replace_by_value_not_found(void);
void test_remove_by_value(void);
void test_count_remove(void);
void test_count_replace(void);
void test_add_item(void);
void test_remove(void);
void test_destructor(void);
void test_pop_push(void);
void test_swap(void);

void test_compare_type_integer(void);
void test_compare_type_float(void);
void test_compare_type_char(void);
void test_compare_type_double(void);
void test_compare_type_string(void);
void test_compare_type_pointer(void);
void test_pointer_custom_comparator(void);
void test_node_get_set_data_null(void);

void test_to_array(void);
void test_iterator(void);
void test_iterator_get_set(void);
void test_iterator_edge_cases(void);
void test_iterator_empty_list(void);
void test_sort_invalid_params(void);
void test_sort(void);
void test_sort_reverse(void);
void test_shuffle(void);
void test_helper_get(void);

void test_print_list_empty(void);
void test_print_list_integer(void);
void test_print_list_custom_separator(void);
void test_print_list_string(void);
void test_print_list_float_double(void);
void test_print_list_pointer(void);
void test_print_list_reverse(void);
void test_print_list_reverse_empty(void);

int main(void) {
  srand(SEED);

  UNITY_BEGIN();

  RUN_TEST(test_empty_list);

  RUN_TEST(test_populate_list);
  RUN_TEST(test_populate_type_integer);
  RUN_TEST(test_populate_type_float);
  RUN_TEST(test_populate_type_char);
  RUN_TEST(test_populate_type_double);
  RUN_TEST(test_populate_type_string);

  RUN_TEST(test_append_prepend);
  RUN_TEST(test_push_prepend_empty_and_single);

  RUN_TEST(test_replace);
  RUN_TEST(test_replace_by_value);
  RUN_TEST(test_replace_by_value_string);
  RUN_TEST(test_replace_by_value_not_found);
  RUN_TEST(test_remove_by_value);
  RUN_TEST(test_count_remove);
  RUN_TEST(test_count_replace);
  RUN_TEST(test_add_item);

  RUN_TEST(test_compare_type_integer);
  RUN_TEST(test_compare_type_float);
  RUN_TEST(test_compare_type_char);
  RUN_TEST(test_compare_type_double);
  RUN_TEST(test_compare_type_string);
  RUN_TEST(test_compare_type_pointer);
  RUN_TEST(test_pointer_custom_comparator);

  RUN_TEST(test_node_get_set_data_null);

  RUN_TEST(test_remove);
  RUN_TEST(test_pop_push);

  RUN_TEST(test_destructor);

  RUN_TEST(test_swap);

  RUN_TEST(test_to_array);

  RUN_TEST(test_iterator);
  RUN_TEST(test_iterator_get_set);
  RUN_TEST(test_iterator_edge_cases);
  RUN_TEST(test_iterator_empty_list);

  RUN_TEST(test_sort_invalid_params);
  RUN_TEST(test_sort);
  RUN_TEST(test_sort_reverse);
  RUN_TEST(test_shuffle);

  RUN_TEST(test_helper_get);

  RUN_TEST(test_print_list_empty);
  RUN_TEST(test_print_list_integer);
  RUN_TEST(test_print_list_custom_separator);
  RUN_TEST(test_print_list_string);
  RUN_TEST(test_print_list_float_double);
  RUN_TEST(test_print_list_pointer);
  RUN_TEST(test_print_list_reverse);
  RUN_TEST(test_print_list_reverse_empty);

  return UNITY_END();
}
