#pragma once

// Performs an HTTP GET request and returns either a dynamically allocated buffer containing the response body, or NULL
char *http_get(const char *url, long *status_code);
