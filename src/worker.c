#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "worker.h"
#include "database.h"
#include "http.h"

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

    printf("%s\n", response);

    free(response);

    database_close();

    return EXIT_SUCCESS;
}
