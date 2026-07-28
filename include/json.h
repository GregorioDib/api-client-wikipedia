#pragma once

// Internal representation of a Wikipedia article. The metadata required is extracted and the full JSON payload is preserved for storage in the database
typedef struct{
    char title[256];
    char asset_uri[512];
    char description[256];
    char *meta_payload;
} wiki_article_t;

// Parses a Wikipedia JSON response
int json_parse_article(const char *json, wiki_article_t *article);

// Releases dynamically allocated memory owned by a wiki_article_t.
void wiki_article_destroy(wiki_article_t *article);
