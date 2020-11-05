#ifndef _ASYNC_HTTP_REQUEST_H
#define _ASYNC_HTTP_REQUEST_H

struct async_http_attr;
typedef struct async_http_attr async_http_attr_t;

struct async_http_request;
typedef struct async_http_request async_http_request_t;

enum async_http_request_exit {
    ASYNC_HTTP_REQUEST_OK = 0,
    ASYNC_HTTP_REQUEST_FAILED_TO_START = 1,
};

typedef enum async_http_request_exit async_http_request_exit_t;

enum async_http_request_state {
    ASYNC_GET_REQ_RUNNING = 1,
    ASYNC_GET_REQ_COMPLETED = 2,
    ASYNC_GET_REQ_TIMEOUTED = 3,
    ASYNC_GET_REQ_FAILED = 4,
};

typedef enum async_http_request_state async_http_request_state_t;

async_http_request_t *async_http_request_init(async_http_attr_t *attr);

async_http_request_exit_t async_http_request_run(async_http_request_t *request);
async_http_request_state_t async_http_request_wait(async_http_request_t *request, int timeout);

const char * async_http_request_getresponse(async_http_request_t *request);
long async_http_request_getresponsecode(async_http_request_t *request);
double async_http_request_gettotaltime(async_http_request_t *request);
async_http_request_state_t async_http_request_getstate(async_http_request_t *request);

void async_http_request_destroy(async_http_request_t *request);

#endif // _ASYNC_HTTP_REQUEST_H
