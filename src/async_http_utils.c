#include "async_http_utils.h"

#include <curl/curl.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

CURL *async_http_utils_curl_easy_init(void) {
    CURL *easy_handle = curl_easy_init();
    async_http_utils_assert(NULL != easy_handle, "curl_easy_init(): Out of memory");
    return easy_handle;
}

CURLM *async_http_utils_curl_multi_init(void) {
    CURLM *multi_handle = curl_multi_init();
    async_http_utils_assert(NULL != multi_handle, "curl_multi_init(): Out of memory");
    return multi_handle;
}
#include <stdio.h>
void async_http_utils_curl_easy_setopt(CURL *handle, CURLoption option, ...) {
    va_list args;
    va_start(args, option);
    CURLcode code = curl_easy_setopt(handle, option, va_arg(args, void *));
    va_end(args);
    async_http_utils_assert(CURLE_OK == code, "curl_easy_setopt(): %s", curl_easy_strerror(code));
}

void async_http_utils_curl_easy_getinfo(CURL *handle,  CURLINFO info, ...) {
    va_list args;
    va_start(args, info);
    CURLcode code = curl_easy_getinfo(handle, info, va_arg(args, void *));
    va_end(args);
    async_http_utils_assert(CURLE_OK == code, "curl_easy_setopt(): %s", curl_easy_strerror(code));
}

void async_http_utils_curl_multi_add_handle(CURLM *multi_handle, CURL *easy_handle) {
    CURLMcode code = curl_multi_add_handle(multi_handle, easy_handle);
    async_http_utils_assert(CURLM_OK == code, "curl_multi_add_handle(): %s", curl_multi_strerror(code));
}

void async_http_utils_curl_multi_remove_handle(CURLM *multi_handle, CURL *easy_handle) {
    CURLMcode code = curl_multi_remove_handle(multi_handle, easy_handle);
    async_http_utils_assert(CURLM_OK == code, "curl_multi_remove_handle(): %s", curl_multi_strerror(code));
}

void *async_http_utils_malloc(size_t size) {
    void *general_pointer = malloc(size);
    //fprintf(stderr, "malloc(%lu) -> %p\n", size, general_pointer);
    async_http_utils_assert(NULL != general_pointer, "malloc(): %s", strerror(errno));
    return general_pointer;
}

void *async_http_utils_realloc(void *ptr, size_t size) {
    void *general_pointer = realloc(ptr, size);
    //fprintf(stderr, "realloc(%p, %lu) -> %p\n", ptr, size, general_pointer);
    async_http_utils_assert(NULL != general_pointer, "realloc(): %s", strerror(errno));
    return general_pointer;
}

char *async_http_utils_strdup(const char *s) {
    char *duplicated_string = strdup(s);
    //fprintf(stderr, "strdup(%s) -> %p\n", s, duplicated_string);
    async_http_utils_assert(NULL != duplicated_string, "strdup(): %s", strerror(errno));
    return duplicated_string;
}

void async_http_utils_free(void *ptr) {
    //fprintf(stderr, "free(%p)\n", ptr);
    free(ptr);
}
