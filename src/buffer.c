#include "buffer.h"

#include <stdlib.h>
#include <string.h>

size_t buffer_ensure_chunk_multiple(size_t size) {
    size_t rounded = (size + BUFFER_CHUNK_SIZE - 1) & ~(BUFFER_CHUNK_SIZE - 1);
    return rounded | ((rounded == 0) * BUFFER_CHUNK_SIZE);
}

void buffer_init(struct buffer* buffer) {
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

void buffer_destroy(struct buffer* buffer) {
    if (buffer->capacity > 0) {
        free(buffer->data);
    }
    // to avoid double frees, initalize the buffer
    buffer_init(buffer);
}

void buffer_grow(struct buffer* buffer, size_t capacity) {
    capacity = buffer_ensure_chunk_multiple(capacity);
    if (buffer->capacity == 0) {
        buffer->data = malloc(capacity);
    } else {
        buffer->data = realloc(buffer->data, capacity);
    }
    buffer->capacity = capacity;
}

void buffer_append(struct buffer* buffer, void* data, size_t length) {
    if (buffer->size + length > buffer->capacity) {
        buffer_grow(buffer, buffer->size + length);
    }
    memcpy(&buffer->data[buffer->size], data, length);
    buffer->size += length;
}
