#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "worker.h"
#include "database.h"

int worker_run(int argc, char *argv[]){

    (void)argc;
    (void)argv;

    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);

    if (database_init() != 0) {
        return EXIT_FAILURE;
    }
    
    printf("Worker initialized.\n");

    database_close();

    return EXIT_SUCCESS;
}
