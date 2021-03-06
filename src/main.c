#include "async_http.h"

#include <stdio.h>

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <url>\n", argv[0]);
        return 1;
    }

    async_http_attr_t *attr = async_http_attr_init();
    async_http_attr_seturl(attr, argv[1]);

    async_http_request_t *request = async_http_request_init(attr);
    async_http_request_state_t state = async_http_request_start(request);

    while (ASYNC_HTTP_REQUEST_RUNNING == state) {
        // do things in the meantime
        state = async_http_request_wait(request, 10);
    }

    if (ASYNC_HTTP_REQUEST_DONE == state) {
        fprintf(stderr, "response code: %ld\n", async_http_request_getresponsecode(request));
        fprintf(stderr, "response: %s\n", async_http_request_getresponse(request));
        fprintf(stderr, "total time: %g\n", async_http_request_gettotaltime(request));
        fprintf(stderr, "server ip: %s\n", async_http_request_gethostip(request));
    } else {
        fprintf(stderr, "async_http failed with state: %d\n", state);
    }

    async_http_request_destroy(request);
    async_http_attr_destroy(attr);
}
