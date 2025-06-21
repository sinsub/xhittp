#include "test_runner.h"

void setUp() {}

void tearDown() {}

int main() {
    UNITY_BEGIN();

    test_runner_buffer();
    test_runner_http_request();

    return UNITY_END();
}
