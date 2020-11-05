#include "async_http_callbacks.h"
#include "async_http_utils.h"

#include <string.h>

struct async_http_callbacks_response {
    char *data;
    size_t size;
};

size_t async_http_callbacks_write(void *contents, size_t size, size_t nmemb, void *userp) {
    async_http_callbacks_response_t *data = (async_http_callbacks_response_t *) userp;
    size_t real_size = size * nmemb;
    
    char *ptr = async_http_utils_realloc(data->data, data->size + real_size + 1);

    data->data = ptr;
    memcpy(&(data->data[data->size]), contents, real_size);

    data->size += real_size;
    data->data[data->size] = 0;

    return real_size;
}

async_http_callbacks_response_t *async_http_callbacks_response_init(void) {
    async_http_callbacks_response_t *new_response = (async_http_callbacks_response_t *) async_http_utils_malloc(
        sizeof(async_http_callbacks_response_t)
    );

    new_response->data = (char *) async_http_utils_malloc(1);
    new_response->data[0] = '\0';

    new_response->size = 0;

    return new_response;
}

const char * async_http_callbacks_response_getresponse(async_http_callbacks_response_t *response) {
	return response->data;
}

void async_http_callbacks_response_destroy(async_http_callbacks_response_t *response) {
	free(response->data);
	free(response);
}
