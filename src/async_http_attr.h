#ifndef _ASYNC_HTTP_ATTR_
#define _ASYNC_HTTP_ATTR_

struct async_http_attr;
typedef struct async_http_attr async_http_attr_t;

async_http_attr_t * async_http_attr_init(void);

void async_http_attr_seturl(async_http_attr_t *attr, const char *url);
const char * async_http_attr_geturl(const async_http_attr_t *attr);

void async_http_attr_setproxy(async_http_attr_t *attr, const char *proxy);
const char * async_http_attr_getproxy(const async_http_attr_t *attr);

void async_http_attr_destroy(async_http_attr_t *attr);

#endif // _ASYNC_HTTP_ATTR_
