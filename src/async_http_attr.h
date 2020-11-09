#ifndef _ASYNC_HTTP_ATTR_
#define _ASYNC_HTTP_ATTR_

struct async_http_attr;
typedef struct async_http_attr async_http_attr_t;

/**
 * Create and init async_http_attr_t object.
 *
 * @return a new async_http_attr_t object.
 */
async_http_attr_t * async_http_attr_init(void);

/**
 * Set an url to 'url' (should be non-null).
 *
 * @param attr initialized async_http_attr_t object
 * @param url an url
 */
void async_http_attr_seturl(async_http_attr_t *attr, const char *url);

/**
 * Get an url.
 *
 * Might return NULL if not set.
 *
 * @param attr initialized async_http_attr_t object
 * @return url
 */
const char * async_http_attr_geturl(const async_http_attr_t *attr);

/**
 * Set an proxy to 'proxy' (should be non-null).
 *
 * @param attr initialized async_http_attr_t object
 * @param proxy a proxy
 */
void async_http_attr_setproxy(async_http_attr_t *attr, const char *proxy);

/**
 * Get a proxy.
 *
 * Might return NULL if not set.
 *
 * @param attr initialized async_http_attr_t object
 * @return proxy
 */
const char * async_http_attr_getproxy(const async_http_attr_t *attr);

/**
 * Destroy async_http_attr_t object.
 *
 * This function cleans up internal structures of async_http_attr_t and cancels
 * a running connection if there is any.
 *
 * Note that passed object should not be used anymore after calling *_destroy.
 *
 * @param attr async_http_attr_t object
 */
void async_http_attr_destroy(async_http_attr_t *attr);

#endif // _ASYNC_HTTP_ATTR_
