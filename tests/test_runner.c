#include "all_tests.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();
    // buffer
    RUN_TEST(test_buffer_chunk);
    RUN_TEST(test_buffer_init);
    RUN_TEST(test_buffer_grow);
    RUN_TEST(test_buffer_append);
    RUN_TEST(test_buffer_destory);
    return UNITY_END();
}