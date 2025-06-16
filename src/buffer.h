#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

// Ensure this is a power of 2
#define BUFFER_CHUNK_SIZE 4096
size_t buffer_ensure_chunk_multiple(size_t size);

struct buffer {
    char* data;
    size_t size;
    size_t capacity;
};

void buffer_init(struct buffer* buffer);
void buffer_destroy(struct buffer* buffer);
void buffer_grow(struct buffer* buffer, size_t capacity);
void buffer_append(struct buffer* buffer, void* data, size_t length);

#endif