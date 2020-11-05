#include "async_http_request.h"
#include "async_http_utils.h"
#include "async_http_callbacks.h"
#include "async_http_attr.h"

#include <stdlib.h>

#include <curl/curl.h>

struct async_http_request {
    CURL *easy_handle;
    CURLM *multi_handle;

    async_http_callbacks_response_t *response;
};

static void _async_http_request_init_internal(async_http_request_t *request, async_http_attr_t *attr);
static void _async_http_request_destroy_internal(async_http_request_t *request);

async_http_request_t *async_http_request_init(async_http_attr_t *attr) {
    async_http_request_t *new_request = (async_http_request_t *) async_http_utils_malloc(
        sizeof(async_http_request_t)
    );

    _async_http_request_init_internal(new_request, attr);

    return new_request;
}

async_http_request_exit_t async_http_request_run(async_http_request_t *request) {
    int running_handles;
    CURLMcode code = curl_multi_perform(request->multi_handle, &running_handles);

    if (CURLM_OK == code) {
        fprintf(stderr, "%s\n", "fuck yeah");
        if (0 == running_handles) {
            return ASYNC_HTTP_REQUEST_FAILED_TO_START;
        } else {
            return ASYNC_HTTP_REQUEST_OK;
        }
    }
    
    return ASYNC_HTTP_REQUEST_FAILED_TO_START;
}

async_http_request_state_t async_http_request_wait(async_http_request_t *request, int timeout) {
    int numfds;
    curl_multi_wait(request->multi_handle, NULL, 0, timeout, &numfds);

    int running_handles;
    CURLMcode code = curl_multi_perform(request->multi_handle, &running_handles);

    if (CURLM_OK == code) {
        if (0 == running_handles) {
            return ASYNC_GET_REQ_COMPLETED;
        } else {
            return ASYNC_GET_REQ_RUNNING;
        }
    }
    
    return ASYNC_GET_REQ_FAILED;
}

const char * async_http_request_getresponse(async_http_request_t *request) {
    return async_http_callbacks_response_getresponse(request->response);
}

long async_http_request_getresponsecode(async_http_request_t *request) {
    long response_code;
    async_http_utils_curl_easy_getinfo(request->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    return response_code;
}

double async_http_request_gettotaltime(async_http_request_t *request) {
    double total_time;
    async_http_utils_curl_easy_getinfo(request->easy_handle, CURLINFO_TOTAL_TIME, &total_time);
    return total_time;
}

async_http_request_state_t async_http_request_getstate(async_http_request_t *request) {
    int running_handles;
    curl_multi_perform(request->multi_handle, &running_handles);

    return ASYNC_GET_REQ_RUNNING;
}

void async_http_request_destroy(async_http_request_t *request) {
    _async_http_request_destroy_internal(request);
    free(request);
}

static void _async_http_request_init_internal(async_http_request_t *request, async_http_attr_t *attr) {
    request->easy_handle = async_http_utils_curl_easy_init();
    request->multi_handle = async_http_utils_curl_multi_init();

    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_URL, (char *) async_http_attr_geturl(attr));
    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_PROXY, (char *) async_http_attr_getproxy(attr));

    request->response = async_http_callbacks_response_init();

    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_WRITEFUNCTION, async_http_callbacks_write);
    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_WRITEDATA, &request->response);

    async_http_utils_curl_multi_add_handle(request->multi_handle, request->easy_handle);
}
#include<stdio.h>
static void _async_http_request_destroy_internal(async_http_request_t *request) {
    curl_multi_remove_handle(request->multi_handle, request->easy_handle);
    curl_easy_cleanup(request->easy_handle);
    curl_multi_cleanup(request->multi_handle);
    async_http_callbacks_response_destroy(request->response);
}
