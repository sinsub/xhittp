#include "test_runner.h"

#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    test_runner_buffer();

    return UNITY_END();
}
