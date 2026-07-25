#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "worker.h"
#include "database.h"
#include "http.h"
#include "json.h"
#include "persistence.h"

int worker_run(int argc, char *argv[]){

    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);

    if (argc != 2){
        fprintf(stderr, "Usage: %s <article>\n", PROGRAM_NAME);
        return EXIT_FAILURE;
    }
    
    if (database_init() != 0) {
        return EXIT_FAILURE;
    }
    
    
    // Build the wikipedia request URL
    char url[512];

    snprintf(url, sizeof(url), "%s%s", WIKIPEDIA_API_BASE, argv[1]);

    long status;
    
    //Downloads the article
    char *response = http_get(url, &status);

    if (response == NULL){
        database_close();
        return EXIT_FAILURE;
    }

    printf("HTTP Status: %ld\n\n", status);

    //Parses JSON response
    wiki_article_t article;

    if (json_parse_article(response, &article) != 0){
        fprintf(stderr, "Unable to parse Wikipedia response.\n");
        free(response);
        database_close();
        return EXIT_FAILURE;
    }

    // Display of extracted metadata
    printf("Title       : %s\n", article.title);
    printf("Description : %s\n", article.description);
    printf("URI         : %s\n\n", article.asset_uri);

    // Persistence of the article
    printf("Persisting article...\n");

    long long asset_id = persistence_save_article(&article);

    if (asset_id < 0){
        fprintf(stderr, "Unable to persist article.\n");

        wiki_article_destroy(&article);
        free(response);
        database_close();

        return EXIT_FAILURE;
    }

    printf("Article stored successfully.\n");
    printf("Asset ID    : %lld\n", asset_id);

    //Cleanup
    wiki_article_destroy(&article);
    free(response);

    database_close();

    return EXIT_SUCCESS;
}
