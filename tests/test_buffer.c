#include <buffer.h>

#include "test_runner.h"

void test_buffer_chunk() {
    size_t expected = BUFFER_CHUNK_SIZE;
    TEST_ASSERT_EQUAL(expected, buffer_ensure_chunk_multiple(0));
    TEST_ASSERT_EQUAL(expected, buffer_ensure_chunk_multiple(1));
    TEST_ASSERT_EQUAL(expected, buffer_ensure_chunk_multiple(BUFFER_CHUNK_SIZE));

    expected = BUFFER_CHUNK_SIZE + BUFFER_CHUNK_SIZE;

    TEST_ASSERT_EQUAL(expected, buffer_ensure_chunk_multiple(BUFFER_CHUNK_SIZE + 1));
    TEST_ASSERT_EQUAL(expected,
                      buffer_ensure_chunk_multiple(BUFFER_CHUNK_SIZE + BUFFER_CHUNK_SIZE));
}

void test_buffer_init() {
    struct buffer buffer;

    // fill with random data
    buffer.size = 10;
    buffer.capacity = 10;
    buffer.data = (char *)0x10;

    buffer_init(&buffer);
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(0, buffer.capacity);
    TEST_ASSERT_EQUAL(0, buffer.data);
}

void test_buffer_grow() {
    struct buffer buffer;
    buffer_init(&buffer);

    buffer_grow(&buffer, 10);
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(BUFFER_CHUNK_SIZE, buffer.capacity);
    TEST_ASSERT_NOT_EQUAL(0, buffer.data);

    buffer_grow(&buffer, BUFFER_CHUNK_SIZE + 1);
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(2 * BUFFER_CHUNK_SIZE, buffer.capacity);
    TEST_ASSERT_NOT_EQUAL(0, buffer.data);
}

void test_buffer_append() {
    struct buffer buffer;
    buffer_init(&buffer);

    // purposefully not null-terminated
    char hello[5] = "hello";
    char world[5] = "world";

    buffer_append(&buffer, hello, 5);
    TEST_ASSERT_EQUAL(5, buffer.size);
    TEST_ASSERT_EQUAL(BUFFER_CHUNK_SIZE, buffer.capacity);
    TEST_ASSERT_NOT_EQUAL(0, buffer.data);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(hello, buffer.data, 5);

    buffer_append(&buffer, world, 5);
    TEST_ASSERT_EQUAL(10, buffer.size);
    TEST_ASSERT_EQUAL(BUFFER_CHUNK_SIZE, buffer.capacity);
    TEST_ASSERT_NOT_EQUAL(0, buffer.data);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(hello, buffer.data, 5);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(world, &buffer.data[5], 5);
}

void test_buffer_destory() {
    struct buffer buffer;
    buffer_init(&buffer);

    // purposefully not null-terminated
    char hello[5] = "hello";
    buffer_append(&buffer, hello, 5);

    buffer_destroy(&buffer);
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(0, buffer.capacity);
    TEST_ASSERT_EQUAL(0, buffer.data);
}

void test_runner_buffer() {
    RUN_TEST(test_buffer_chunk);
    RUN_TEST(test_buffer_init);
    RUN_TEST(test_buffer_grow);
    RUN_TEST(test_buffer_append);
    RUN_TEST(test_buffer_destory);
}
