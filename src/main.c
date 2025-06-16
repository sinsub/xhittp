#include <stdio.h>
#include <string.h>

#include "buffer.h"

int main() {
    struct buffer buffer;
    buffer_init(&buffer);

    char hello[] = "hello";
    char world[] = "world";

    buffer_append(&buffer, hello, strlen(hello));
    buffer_append(&buffer, world, strlen(world) + 1);  // include the '\0'

    printf("buffer contents: %s\n", buffer.data);
    printf("buffer size: %zu\n", buffer.size);
    printf("buffer capacity: %zu\n", buffer.capacity);

    buffer_destroy(&buffer);
}
