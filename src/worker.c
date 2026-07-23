#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "worker.h"
#include "database.h"
#include "http.h"
#include "json.h"

int worker_run(int argc, char *argv[]){

    (void)argc;
    (void)argv;

    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);

    if (database_init() != 0) {
        return EXIT_FAILURE;
    }
    
    
    // PLACEHOLDER - Build the request URL
    char url[512];

    snprintf(url, sizeof(url), "%sLinux", WIKIPEDIA_API_BASE);

    long status;

    char *response = http_get(url, &status);

    if (response == NULL){
        database_close();
        return EXIT_FAILURE;
    }

    printf("HTTP Status: %ld\n\n", status);

    wiki_article_t article;

    if (json_parse_article(response, &article) != 0){
        fprintf(stderr, "Unable to parse Wikipedia response.\n");
        free(response);
        database_close();
        return EXIT_FAILURE;
    }

    //Display of metadata

    printf("Title       : %s\n", article.title);
    printf("Description : %s\n", article.description);
    printf("URI         : %s\n", article.asset_uri);
    printf("Payload size: %zu bytes\n\n", strlen(article.meta_payload));

    wiki_article_destroy(&article);
    free(response);
    database_close();
    return EXIT_SUCCESS;
}
