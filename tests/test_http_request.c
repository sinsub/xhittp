#include <http_request.h>

#include "test_runner.h"

void test_http_request_init() {
    struct http_request request;
    http_request_init(&request);
    TEST_ASSERT_EQUAL(HRS_METHOD, request.state);
    TEST_ASSERT_EQUAL(NULL, request.buffer.data);
    TEST_ASSERT_EQUAL(0, request.buffer.size);
    TEST_ASSERT_EQUAL(0, request.buffer.capacity);
    TEST_ASSERT_EQUAL(0, request.parser_context.start);
    TEST_ASSERT_EQUAL(0, request.parser_context.current);
    TEST_ASSERT_EQUAL(NULL, request.method);
}

void test_http_request_read_incomplete_method() {
    struct http_request request;
    http_request_init(&request);
    char buffer[3] = {'G', 'E', 'T'};

    http_request_read(&request, buffer, 0);
    TEST_ASSERT_EQUAL(HRS_METHOD, request.state);
    TEST_ASSERT_EQUAL(NULL, request.buffer.data);
    TEST_ASSERT_EQUAL(0, request.buffer.size);
    TEST_ASSERT_EQUAL(0, request.buffer.capacity);
    TEST_ASSERT_EQUAL(0, request.parser_context.start);
    TEST_ASSERT_EQUAL(0, request.parser_context.current);
    TEST_ASSERT_EQUAL(NULL, request.method);

    http_request_read(&request, buffer, 1);
    TEST_ASSERT_EQUAL(HRS_METHOD, request.state);
    TEST_ASSERT_NOT_EQUAL(NULL, request.buffer.data);
    TEST_ASSERT_EQUAL(1, request.buffer.size);
    TEST_ASSERT_EQUAL(buffer[0], request.buffer.data[0]);
    TEST_ASSERT_EQUAL(BUFFER_CHUNK_SIZE, request.buffer.capacity);
    TEST_ASSERT_EQUAL(0, request.parser_context.start);
    TEST_ASSERT_EQUAL(1, request.parser_context.current);
    TEST_ASSERT_EQUAL(NULL, request.method);

    http_request_read(&request, &buffer[1], 2);
    TEST_ASSERT_EQUAL(HRS_METHOD, request.state);
    TEST_ASSERT_NOT_EQUAL(NULL, request.buffer.data);
    TEST_ASSERT_EQUAL(3, request.buffer.size);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buffer, request.buffer.data, 3);
    TEST_ASSERT_EQUAL(BUFFER_CHUNK_SIZE, request.buffer.capacity);
    TEST_ASSERT_EQUAL(0, request.parser_context.start);
    TEST_ASSERT_EQUAL(3, request.parser_context.current);
    TEST_ASSERT_EQUAL(NULL, request.method);
}

void test_http_request_read_method_get() {
    struct http_request request;
    http_request_init(&request);
    char buffer[] = {'G', 'E', 'T', ' '};
    char method[] = "GET";
    size_t size = sizeof(buffer) / sizeof(buffer[0]);

    http_request_read(&request, buffer, size);
    TEST_ASSERT_EQUAL(HRS_URI, request.state);
    TEST_ASSERT_EQUAL(size, request.buffer.size);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(method, request.buffer.data, size);
    TEST_ASSERT_EQUAL(size, request.parser_context.start);
    TEST_ASSERT_EQUAL(size, request.parser_context.current);
    TEST_ASSERT_EQUAL(request.buffer.data, request.method);
}

void test_http_request_read_method_head() {
    struct http_request request;
    http_request_init(&request);
    char buffer[] = {'H', 'E', 'A', 'D', ' '};
    char method[] = "HEAD";
    size_t size = sizeof(buffer) / sizeof(buffer[0]);

    http_request_read(&request, buffer, size);
    TEST_ASSERT_EQUAL(HRS_URI, request.state);
    TEST_ASSERT_EQUAL(size, request.buffer.size);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(method, request.buffer.data, size);
    TEST_ASSERT_EQUAL(size, request.parser_context.start);
    TEST_ASSERT_EQUAL(size, request.parser_context.current);
    TEST_ASSERT_EQUAL(request.buffer.data, request.method);
}

void test_http_request_read_method_unsupported() {
    struct http_request request;
    http_request_init(&request);
    char buffer[] = {'R', 'I', 'Z', 'Z', ' '};
    char method[] = "RIZZ";
    size_t size = sizeof(buffer) / sizeof(buffer[0]);

    http_request_read(&request, buffer, size);
    TEST_ASSERT_EQUAL(HRS_ERROR, request.state);
    TEST_ASSERT_EQUAL(size, request.buffer.size);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(method, request.buffer.data, size);
    TEST_ASSERT_EQUAL(size, request.parser_context.start);
    TEST_ASSERT_EQUAL(size, request.parser_context.current);
    TEST_ASSERT_EQUAL(request.buffer.data, request.method);
}

void test_http_request_read_method_preeceding_space() {
    struct http_request request;
    http_request_init(&request);
    char buffer[] = {' ', 'G', 'E', 'T', ' '};
    size_t size = sizeof(buffer) / sizeof(buffer[0]);

    http_request_read(&request, buffer, size);
    TEST_ASSERT_EQUAL(HRS_ERROR, request.state);
    // no need to read past the first character
    TEST_ASSERT_EQUAL(1, request.parser_context.current);
}

void test_http_request_read_method_preeceding_tab() {
    struct http_request request;
    http_request_init(&request);
    char buffer[] = {'\t', 'G', 'E', 'T', ' '};
    size_t size = sizeof(buffer) / sizeof(buffer[0]);

    http_request_read(&request, buffer, size);
    TEST_ASSERT_EQUAL(HRS_ERROR, request.state);
    // no need to read past the first character
    TEST_ASSERT_EQUAL(1, request.parser_context.current);
}

void test_runner_http_request() {
    RUN_TEST(test_http_request_init);
    RUN_TEST(test_http_request_read_incomplete_method);
    RUN_TEST(test_http_request_read_method_get);
    RUN_TEST(test_http_request_read_method_head);
    RUN_TEST(test_http_request_read_method_unsupported);
    RUN_TEST(test_http_request_read_method_preeceding_space);
    RUN_TEST(test_http_request_read_method_preeceding_tab);
}