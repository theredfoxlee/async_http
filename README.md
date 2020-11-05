# async_http

async_http - deadly simple C API for making asynchronous HTTP requests.

## notes

async_http API uses abort() policy when memory fails to be allocated in some glib functions or some libcurl internal error happens.

It's a bad design decision if you'd like to perform recovery from that state, b**ut it simplifies an API a lot**.

While using most of async_http_* functions, you can be sure that they succeed (no error checking/propagation is required).

## usage

```c
#include "async_get_attr.h"
#include "async_get_request.h"

#include <stdio.h>

int
main() {
    async_get_attr_t *attr = async_get_attr_init();

    async_get_attr_seturl(attr, "htpp://google.com");
    async_get_attr_setproxy(attr, "htpp://proxy.com:3389");

    async_get_request_t *request = async_get_request_init(attr);

    if (ASYNC_GET_REQUEST_OK == async_get_request_run(request)) {
        do {
            // do things in the meantime
        } while (ASYNC_GET_REQUEST_RUNNING == async_get_request_wait(req, 10));
        
        if (ASYNC_GET_REQUEST_COMPLETED == async_get_request_getstate(req)) {
            fprintf(stderr, "http status: %u\n", async_get_request_gethttpstatus(req));
            fprintf(stderr, "total time: %u\n", async_get_request_gettotaltime(req));
            fprintf(stderr, "response: %s\n", async_get_request_getresponse(req));
        }
    }
    
    async_get_request_destroy(request);

    async_get_attr_destroy(attr);
}
```

