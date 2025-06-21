#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "buffer.h"

enum http_request_state { HRS_METHOD, HRS_URI, HRS_HTTP_VERSION, HRS_ERROR, HRS_DONE };

struct parser_context {
    size_t start;
    size_t current;
};

struct http_request {
    enum http_request_state state;
    struct buffer buffer;
    struct parser_context parser_context;
    char *method;
};

void http_request_init(struct http_request *request);
void http_request_destory(struct http_request *request);
void http_request_read(struct http_request *request, void *buffer, size_t length);

#endif
