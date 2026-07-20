#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "config.h"
#include "http.h"

//  Internal buffer used to accumulate the HTTP response.
typedef struct{
    char *data;
    size_t size;
} http_buffer_t;

/*
 * libcurl write callback.
 *
 * This function is called by libcurl whenever a new chunk of data
 * is received. The buffer is resized dynamically using realloc.
 */
static size_t write_callback(void *contents, size_t size,
                             size_t nmemb, void *userp){
    size_t total_size = size * nmemb;

    http_buffer_t *buffer = (http_buffer_t *)userp;

    char *new_data = realloc(buffer->data, buffer->size + total_size + 1);
    if (new_data == NULL) {
        fprintf(stderr, "Failed to allocate memory for HTTP response.\n");
        return 0; /* Returning 0 tells libcurl to abort the transfer */
    }

    buffer->data = new_data;

    memcpy(buffer->data + buffer->size, contents, total_size);

    buffer->size += total_size;
    buffer->data[buffer->size] = '\0';

    return total_size;
}

//  Performs an HTTP GET request using libcurl.
char *http_get(const char *url, long *status_code){
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Failed to initialize libcurl.\n");
        return NULL;
    }

    http_buffer_t buffer = {
        .data = malloc(1),
        .size = 0
    };

    if (buffer.data == NULL) {
        fprintf(stderr, "Failed to allocate initial response buffer.\n");
        curl_easy_cleanup(curl);
        return NULL;
    }

    buffer.data[0] = '\0';

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Identify the client application
    curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);

    // Follow HTTP redirects automatically
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Set connection and request timeouts
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

    // Configure response handling
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "HTTP request failed: %s\n",
                curl_easy_strerror(res));

        free(buffer.data);
        curl_easy_cleanup(curl);

        return NULL;
    }

    // Retrieve the HTTP status code
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, status_code);

    curl_easy_cleanup(curl);

    return buffer.data;
}
