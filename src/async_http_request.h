#ifndef _ASYNC_HTTP_REQUEST_H
#define _ASYNC_HTTP_REQUEST_H

struct async_http_attr;
typedef struct async_http_attr async_http_attr_t;

struct async_http_request;
typedef struct async_http_request async_http_request_t;

enum async_http_request_state {
	ASYNC_HTTP_REQUEST_DONE = 0,
	ASYNC_HTTP_REQUEST_RUNNING = 1,
	ASYNC_HTTP_REQUEST_FAILED = 2,
	ASYNC_HTTP_REQUEST_NOT_STARTED = 3
};

typedef enum async_http_request_state async_http_request_state_t;

async_http_request_t *async_http_request_init(async_http_attr_t *attr);

async_http_request_state_t async_http_request_run(async_http_request_t *request);
async_http_request_state_t async_http_request_wait(async_http_request_t *request, int timeout);

const char * async_http_request_getresponse(const async_http_request_t *request);
long async_http_request_getresponsecode(const async_http_request_t *request);
double async_http_request_gettotaltime(const async_http_request_t *request);
async_http_request_state_t async_http_request_getstate(const async_http_request_t *request);

void async_http_request_destroy(async_http_request_t *request);

#endif // _ASYNC_HTTP_REQUEST_H
