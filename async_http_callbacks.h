#ifndef _ASYNC_HTTP_CALLBACKS_
#define _ASYNC_HTTP_CALLBACKS_

#include <stddef.h>

struct async_http_callbacks_response;
typedef struct async_http_callbacks_response async_http_callbacks_response_t;

size_t async_http_callbacks_write(void *contents, size_t size, size_t nmemb, void *userp);

async_http_callbacks_response_t *async_http_callbacks_response_init(void);
const char * async_http_callbacks_response_getresponse(async_http_callbacks_response_t *response);
void async_http_callbacks_response_destroy(async_http_callbacks_response_t *response);

#endif // _ASYNC_HTTP_CALLBACKS_
