#include "async_http_request.h"
#include "async_http_utils.h"
#include "async_http_callbacks.h"
#include "async_http_attr.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <curl/curl.h>

struct async_http_request {
    CURL *easy_handle;
    CURLM *multi_handle;

    async_http_request_state_t state;

    async_http_callbacks_response_t *response;
};

static void _async_http_request_init_internal(async_http_request_t *request, async_http_attr_t *attr);
static void _async_http_request_reset_internal(async_http_request_t *request);
static void _async_http_request_destroy_internal(async_http_request_t *request);

static async_http_request_state_t _async_http_request_perform(async_http_request_t *request, int has_just_started);
static async_http_request_state_t _async_http_request_wait(async_http_request_t *request, int timeout_ms);

static long long _get_current_timestamp_ms(void);

async_http_request_t *async_http_request_init(async_http_attr_t *attr) {
    async_http_request_t *new_request = (async_http_request_t *) async_http_utils_malloc(
        sizeof(async_http_request_t)
    );
    _async_http_request_init_internal(new_request, attr);
    return new_request;
}

async_http_request_state_t async_http_request_start(async_http_request_t *request) {
    if (ASYNC_HTTP_REQUEST_RUNNING == request->state) {
        return request->state;
    }
    if (ASYNC_HTTP_REQUEST_NOT_STARTED != request->state) {
        // Reset connection when if it was already done.
        _async_http_request_reset_internal(request);
    }
    request->state = _async_http_request_perform(request, 1);
    return request->state;
}

void async_http_request_stop(async_http_request_t *request) {
    if (ASYNC_HTTP_REQUEST_RUNNING == request->state) {
        _async_http_request_reset_internal(request);
    }
}

async_http_request_state_t async_http_request_wait(async_http_request_t *request, int timeout_ms) {
    if (ASYNC_HTTP_REQUEST_RUNNING != request->state) {
        return request->state;
    }
    request->state = _async_http_request_wait(request, timeout_ms);
    return request->state;
}

const char * async_http_request_getresponse(const async_http_request_t *request) {
    return async_http_callbacks_response_getresponse(request->response);
}

long async_http_request_getresponsecode(const async_http_request_t *request) {
    long response_code;
    async_http_utils_curl_easy_getinfo(request->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    return response_code;
}

double async_http_request_gettotaltime(const async_http_request_t *request) {
    double total_time;
    async_http_utils_curl_easy_getinfo(request->easy_handle, CURLINFO_TOTAL_TIME, &total_time);
    return total_time;
}

async_http_request_state_t async_http_request_getstate(const async_http_request_t *request) {
    return request->state;
}

const char *async_http_request_gethostip(const async_http_request_t *request) {
    char *ip = NULL;
    async_http_utils_curl_easy_getinfo(request->easy_handle, CURLINFO_PRIMARY_IP, &ip);
    return (const char *) ip;
}

void async_http_request_destroy(async_http_request_t *request) {
    _async_http_request_destroy_internal(request);
    async_http_utils_free(request);
}

static void _async_http_request_init_internal(async_http_request_t *request, async_http_attr_t *attr) {
    request->easy_handle = async_http_utils_curl_easy_init();
    request->multi_handle = async_http_utils_curl_multi_init();

    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_URL, (char *) async_http_attr_geturl(attr));
    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_PROXY, (char *) async_http_attr_getproxy(attr));

    request->state = ASYNC_HTTP_REQUEST_NOT_STARTED;

    request->response = async_http_callbacks_response_init();

    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_WRITEFUNCTION, async_http_callbacks_write);
    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_WRITEDATA, request->response);

    async_http_utils_curl_multi_add_handle(request->multi_handle, request->easy_handle);
}

static void _async_http_request_reset_internal(async_http_request_t *request) {
    async_http_callbacks_response_destroy(request->response);
    request->response = async_http_callbacks_response_init();

    async_http_utils_curl_easy_setopt(request->easy_handle, CURLOPT_WRITEDATA, request->response);

    request->state = ASYNC_HTTP_REQUEST_NOT_STARTED;

    // the following will cancel running request
    async_http_utils_curl_multi_remove_handle(request->multi_handle, request->easy_handle);
    async_http_utils_curl_multi_add_handle(request->multi_handle, request->easy_handle);
}

static void _async_http_request_destroy_internal(async_http_request_t *request) {
    curl_multi_remove_handle(request->multi_handle, request->easy_handle);
    curl_easy_cleanup(request->easy_handle);
    curl_multi_cleanup(request->multi_handle);
    async_http_callbacks_response_destroy(request->response);
}

static async_http_request_state_t _async_http_request_perform(async_http_request_t *request, int has_just_started) {
    int running_handles;
    CURLMcode code = curl_multi_perform(request->multi_handle, &running_handles);

    while (CURLM_CALL_MULTI_PERFORM == code) {
        code = curl_multi_perform(request->multi_handle, &running_handles);
    }

    if (CURLM_OK != code) {
        fprintf(stderr, "curl_multi_perform(): %s\n", curl_multi_strerror(code));
        return ASYNC_HTTP_REQUEST_FAILED;
    }

    if (0 == running_handles) {
        int msgq;
        struct CURLMsg *m = curl_multi_info_read(request->multi_handle, &msgq);

        if (m && (m->msg == CURLMSG_DONE)) {
            if (CURLE_OK != m->data.result) {
                fprintf(stderr, "curl_multi_perform(): %s\n", curl_easy_strerror(m->data.result));
                return ASYNC_HTTP_REQUEST_FAILED;
            }

            return ASYNC_HTTP_REQUEST_DONE;
        } else if (has_just_started) {
            fprintf(stderr, "curl_multi_perform(): [BUG] no running_handles and nothing done\n");
            return ASYNC_HTTP_REQUEST_FAILED;
        } else {
            return ASYNC_HTTP_REQUEST_DONE;
        }
    } else if (1 != running_handles) {
        fprintf(stderr, "curl_multi_perform(): [BUG] unknown number of running_handles (1 != %d)", running_handles);
        return ASYNC_HTTP_REQUEST_FAILED;
    }

    return ASYNC_HTTP_REQUEST_RUNNING;
}

static async_http_request_state_t _async_http_request_wait(async_http_request_t *request, int timeout_ms) {
    const long long end_timestamp = _get_current_timestamp_ms() + timeout_ms;

    while (1) {
        async_http_request_state_t ret = _async_http_request_perform(request, 0);

        if (ASYNC_HTTP_REQUEST_RUNNING != ret) {
            return ret;
        }

        int numfds;
        CURLMcode code = curl_multi_wait(request->multi_handle, NULL, 0, timeout_ms, &numfds);

        if (CURLM_OK != code) {
            fprintf(stderr, "curl_multi_perform(): %s\n", curl_multi_strerror(code));
            return ASYNC_HTTP_REQUEST_FAILED;
        }

        if (end_timestamp <= _get_current_timestamp_ms()) {
            if (1 == numfds) {
                /* although we have reached the timeout curl_multi_wait
                 * finished with success, so let's download incoming data */
                return _async_http_request_perform(request, 0);
            }
            return ASYNC_HTTP_REQUEST_RUNNING;
        }
    }

    return ASYNC_HTTP_REQUEST_RUNNING;
}

static long long _get_current_timestamp_ms(void) {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // milliseconds
    return milliseconds;
}
