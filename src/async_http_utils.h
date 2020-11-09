#ifndef _ASYNC_HTTP_UTILS_
#define _ASYNC_HTTP_UTILS_

#include <stdlib.h> // abort

#include <curl/curl.h>

/* async_http API uses abort() policy when memory fails to be allocated in some glibc
 * functions or some libcurl internal error happens.
 *
 * It's a bad design decision if you'd like to perform recovery from that state
 * - but it simplifies API a lot.
 * 
 * Use the following functions instead of regular ones - if they finish their execution,
 * you can be sure that they succeded (no error checking/propagation is required).
 */

#define async_http_utils_assert(expression, format, ...) \
    do { \
        if (!(expression)) { \
            fprintf(stderr, "assert (%s) failed at %s:%d:%s() - reason -> " format "\n", \
                    #expression, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            abort(); \
        } \
    } while(0)

CURL *async_http_utils_curl_easy_init(void);
CURLM *async_http_utils_curl_multi_init(void);

void async_http_utils_curl_easy_setopt(CURL *handle, CURLoption option, ...);
void async_http_utils_curl_easy_getinfo(CURL *handle,  CURLINFO info, ...);
void async_http_utils_curl_multi_add_handle(CURLM *multi_handle, CURL *easy_handle);
void async_http_utils_curl_multi_remove_handle(CURLM *multi_handle, CURL *easy_handle);

void *async_http_utils_malloc(size_t size);
void *async_http_utils_realloc(void *ptr, size_t size);
char *async_http_utils_strdup(const char *s);
void async_http_utils_free(void *ptr);

#endif  // _ASYNC_HTTP_UTILS_
