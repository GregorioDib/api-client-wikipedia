#pragma once

/* Performs an HTTP GET request and returns a dynamically allocated buffer
 * containing either the response body or NULL
 *
 * Parameters:
 *   url         URL to request.
 *   status_code Receives the HTTP response code.
 */
char *http_get(const char *url, long *status_code);
