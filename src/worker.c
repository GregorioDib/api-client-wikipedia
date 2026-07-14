#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "worker.h"

int worker_run(int argc, char *argv[]){

    (void)argc;
    (void)argv;

    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
    printf("Worker initialized.\n");

    return EXIT_SUCCESS;
}
