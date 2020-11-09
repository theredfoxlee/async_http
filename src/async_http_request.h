#ifndef _ASYNC_HTTP_REQUEST_H
#define _ASYNC_HTTP_REQUEST_H

struct async_http_attr;
typedef struct async_http_attr async_http_attr_t;

struct async_http_request;
typedef struct async_http_request async_http_request_t;

/**
 * An enumeration that indicates current state of async_http_request_t.
 *
 * It can be obtained via async_http_request_run, async_http_request_wait,
 * and async_http_getstate.
 */
enum async_http_request_state {
	ASYNC_HTTP_REQUEST_DONE = 0,
	ASYNC_HTTP_REQUEST_RUNNING = 1,
	ASYNC_HTTP_REQUEST_FAILED = 2,
	ASYNC_HTTP_REQUEST_NOT_STARTED = 3
};
typedef enum async_http_request_state async_http_request_state_t;

/**
 * Create and init async_http_request_t object.
 *
 * This function utilizes 'attrs', but does not stores any reference to it
 * - async_http_attr_destroy can be called just after async_http_request_init
 *   and nothing wrong will happen.
 *
 * Created object should be cleaned up via async_http_request_destroy.
 *
 * @param attr attributes like HTTP URL or HTTP_PROXY
 * @return a new async_http_request_t object.
 */
async_http_request_t *async_http_request_init(async_http_attr_t *attr);

/**
 * Start async_http_request_t.
 *
 * This function starts fresh http request in an async/non-blocking manner
 * (but it does not do anything if request is already running).
 *
 * It can be called at the beginning of the async_http_request_t object usage
 * or to reuse it.
 *
 * Note that it can fail immediately if an object was initialized with a faulty
 * 'attrs'.
 *
 * @param request initialized async_http_request_t object
 * @return an enumeration containing request state
 */
async_http_request_state_t async_http_request_start(async_http_request_t *request);

/**
 * Stop running async_http_request_t.
 *
 * This function does not have any effect if async_http_request_t is not running.
 *
 * @param request initialized async_http_request_t object
 */
void async_http_request_stop(async_http_request_t *request);

/**
 * Try to finalize async_http_requst_t in a given 'timeout_ms'.
 *
 * This function does not have any effect if async_http_request_t is not running.
 *
 * @param request initialized async_http_request_t object
 * @return an enumeration containing request state
 */
async_http_request_state_t async_http_request_wait(async_http_request_t *request, int timeout_ms);

/**
 * Get a response from the server.
 *
 * It returns empty string ("") by default, so check request state before proceeding.
 *
 * @param request initialized async_http_request_t object
 * @return HTTP text response
 */
const char * async_http_request_getresponse(const async_http_request_t *request);

/**
 * Get an HTTP request response code.
 *
 * It returns 0 by default, so check request state before proceeding.
 *
 * @param request initialized async_http_request_t object
 * @return HTTP response code
 */
long async_http_request_getresponsecode(const async_http_request_t *request);

/**
 * Get a time that took to finalize the request.
 *
 * @param request initialized async_http_request_t object
 * @return total time
 */
double async_http_request_gettotaltime(const async_http_request_t *request);

/**
 * Get last known state of async_http_request_t.
 *
 * @param request async_http_request_t object
 * @return an enumeration containing request state
 */
async_http_request_state_t async_http_request_getstate(const async_http_request_t *request);

/**
 * Destroy async_http_request_t object.
 *
 * This function cleans up internal structures of async_http_request_t and cancels
 * a running connection if there is any.
 *
 * Note that passed object should not be used anymore after calling *_destroy.
 *
 * @param request async_http_request_t object
 */
void async_http_request_destroy(async_http_request_t *request);

#endif // _ASYNC_HTTP_REQUEST_H
