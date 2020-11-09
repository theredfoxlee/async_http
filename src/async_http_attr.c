#include "async_http_attr.h"
#include "async_http_utils.h"

#include <stdlib.h>

struct async_http_attr {
    char *url;
    char *proxy;
};

async_http_attr_t * async_http_attr_init(void) {
    async_http_attr_t *new_attr = (async_http_attr_t *) async_http_utils_malloc(
        sizeof(async_http_attr_t)
    );
    new_attr->url = NULL;
    new_attr->proxy = NULL;
    return new_attr;
}

void async_http_attr_seturl(async_http_attr_t *attr, const char *url) {
    free(attr->url);
    attr->url = async_http_utils_strdup(url);
}
const char * async_http_attr_geturl(const async_http_attr_t *attr) {
    return attr->url;
}

void async_http_attr_setproxy(async_http_attr_t *attr, const char *proxy) {
    free(attr->proxy);
    attr->proxy = async_http_utils_strdup(proxy);
}
const char * async_http_attr_getproxy(const async_http_attr_t *attr) {
    return attr->proxy;
}

void async_http_attr_destroy(async_http_attr_t *attr) {
    free(attr->url);
    free(attr->proxy);
    free(attr);
}
