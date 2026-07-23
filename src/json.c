#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "json.h"

//Copies a JSON string into a fixed-size destination buffer.
static int copy_json_string(cJSON *item, char *destination,size_t destination_size){

    if (!cJSON_IsString(item) || item->valuestring == NULL)
        return -1;

    strncpy(destination, item->valuestring, destination_size - 1);
    destination[destination_size - 1] = '\0';

    return 0;
}

int json_parse_article(const char *json, wiki_article_t *article){

    if (json == NULL || article == NULL){
        return -1;}

    memset(article, 0, sizeof(*article));

    cJSON *root = cJSON_Parse(json);

    if (root == NULL){
        fprintf(stderr, "JSON parse error: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    //Required fields
    cJSON *title = cJSON_GetObjectItemCaseSensitive(root, "title");

    if (copy_json_string(title, article->title, sizeof(article->title)) != 0){
        fprintf(stderr, "Missing required field: title\n");
        cJSON_Delete(root);
        return -1;
    }

    //Description (optional)
    cJSON *description = cJSON_GetObjectItemCaseSensitive(root, "description");

    if (description != NULL && cJSON_IsString(description)){
        copy_json_string(description, article->description, sizeof(article->description));
    }

    // Navigate through content_urls\desktop\page
    cJSON *content_urls = cJSON_GetObjectItemCaseSensitive(root, "content_urls");

    if (!cJSON_IsObject(content_urls)){
        fprintf(stderr, "Missing required field: content_urls\n");
        cJSON_Delete(root);
        return -1;
    }

    cJSON *desktop = cJSON_GetObjectItemCaseSensitive(content_urls, "desktop");

    if (!cJSON_IsObject(desktop)){
        fprintf(stderr, "Missing required field: desktop\n");
        cJSON_Delete(root);
        return -1;
    }

    cJSON *page = cJSON_GetObjectItemCaseSensitive(desktop, "page");

    if (copy_json_string(page, article->asset_uri, sizeof(article->asset_uri)) != 0){
        fprintf(stderr, "Missing required field: page\n");
        cJSON_Delete(root);
        return -1;
    }

    // Preservation of payload (the original response will be stored in the database as meta_payload)
    size_t payload_size = strlen(json) + 1;

    article->meta_payload = malloc(payload_size);

    if (article->meta_payload == NULL){
        fprintf(stderr, "Unable to allocate memory for meta_payload.\n");
        cJSON_Delete(root);
        return -1;
    }

    memcpy(article->meta_payload, json, payload_size);

    //Parsing successful
    cJSON_Delete(root);

    return 0;
}

// Frees dynamically allocated memory owned by a wiki_article_t
void wiki_article_destroy(wiki_article_t *article){
    if (article == NULL){
        return;}

    free(article->meta_payload);
    article->meta_payload = NULL;
}
