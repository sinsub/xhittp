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
    request->response_code = HSC_000;
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
static const size_t max_http_method_length = 4;

static void http_error(struct http_request *request, enum http_status_code code) {
    request->state = HRS_ERROR;
    request->response_code = code;
}

static void validate_method(struct http_request *request) {
    char *method = request->method;
    if (strlen(method)) {
        for (int i = 0; http_methods[i]; ++i) {
            if (strcmp(http_methods[i], method) == 0) {
                request->state = HRS_URI;
                return;
            }
        }
    }
    http_error(request, HSC_501);  // method not supported
}

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
                    validate_method(request);
                } else if (!is_tchar(c)) {
                    http_error(request, HSC_400);  // bad request
                } else if (i - request->parser_context.start > max_http_method_length) {
                    http_error(request, HSC_501);  // method not supported
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
