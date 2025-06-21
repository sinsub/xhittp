#include "http_request.h"

#include <string.h>

#include "http_parser.h"

// forward declaration
static void http_request_continue_parse(struct http_request *request);

void http_request_init(struct http_request *request) {
    request->state = HRS_METHOD;
    buffer_init(&request->buffer);
    request->parser_context.start = 0;
    request->parser_context.current = 0;
    request->method = NULL;
}

void http_request_destory(struct http_request *request) {
    buffer_destroy(&request->buffer);
    http_request_init(request);
}

void http_request_read(struct http_request *request, void *buffer, size_t length) {
    buffer_append(&request->buffer, buffer, length);
    http_request_continue_parse(request);
}

static const char *const http_methods[] = {"GET", "HEAD", NULL};

static enum http_request_state validate_method(char *method) {
    if (strlen(method)) {
        for (int i = 0; http_methods[i]; ++i) {
            if (strcmp(http_methods[i], method) == 0) {
                return HRS_URI;
            }
        }
    }
    return HRS_ERROR;
}

// TODO put a limit on length of method while parsing
static void http_prase_continue_parse_req_line(struct http_request *request) {
    // while there are characters to read
    while (request->state <= HRS_HTTP_VERSION &&
           request->parser_context.current < request->buffer.size) {
        size_t i = request->parser_context.current++;
        char c = request->buffer.data[i];
        switch (request->state) {
            case HRS_METHOD: {
                if (c == ' ') {
                    request->buffer.data[i] = '\0';
                    request->method = &request->buffer.data[request->parser_context.start];
                    request->parser_context.start = request->parser_context.current;
                    request->state = validate_method(request->method);
                } else if (!is_tchar(c)) {
                    request->state = HRS_ERROR;
                }
            } break;
            case HRS_URI:
            case HRS_HTTP_VERSION:
            default:
                return;
        }
    }
}

void http_request_continue_parse(struct http_request *request) {
    http_prase_continue_parse_req_line(request);
}
