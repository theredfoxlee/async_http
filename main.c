#include "async_http_request.h"
#include "async_http_attr.h"

#include <stdio.h>

int
main() {
    async_http_attr_t *attr = async_http_attr_init();
    async_http_attr_seturl(attr, "http://localhost:8000/REVIEW.txt");

    async_http_request_t *request = async_http_request_init(attr);
    async_http_request_state_t state = async_http_request_run(request);

    if (ASYNC_HTTP_REQUEST_RUNNING == state) {
        do {
            // do things in the meantime
            //fprintf(stderr, "Hi.\n");
            state = async_http_request_wait(request, 10);
        } while (ASYNC_HTTP_REQUEST_RUNNING == state);
    }

    if (ASYNC_HTTP_REQUEST_DONE == state) {
        fprintf(stderr, "response code: %ld\n", async_http_request_getresponsecode(request));
        fprintf(stderr, "response: %s\n", async_http_request_getresponse(request));
        fprintf(stderr, "total time: %g\n", async_http_request_gettotaltime(request));
    }

    async_http_request_destroy(request);
    async_http_attr_destroy(attr);
}
