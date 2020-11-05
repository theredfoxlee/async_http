#include "async_http_request.h"
#include "async_http_attr.h"

#include <stdio.h>

int
main() {
	async_http_attr_t *attr = async_http_attr_init();
	async_http_attr_seturl(attr, "http://google.com");

	async_http_request_t *request = async_http_request_init(attr);


	fprintf(stderr, "%d\n", async_http_request_run(request));
	fprintf(stderr, "%d\n", async_http_request_run(request));
	fprintf(stderr, "%d\n", async_http_request_run(request));
	fprintf(stderr, "%d\n", async_http_request_wait(request, 50000));
	fprintf(stderr, "%ld\n", async_http_request_getresponsecode(request));




	async_http_request_destroy(request);
	async_http_attr_destroy(attr);
}