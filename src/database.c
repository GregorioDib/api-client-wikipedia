#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "config.h"
#include "database.h"

/* The database connection remains open during the lifetime of the
 * application and is closed before exiting.*/
static sqlite3 *db = NULL;

static char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open schema file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

int database_init(void){
    if (sqlite3_open(DATABASE_FILE, &db) != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n",
                sqlite3_errmsg(db));

        sqlite3_close(db);
        db = NULL;

        return -1;
    }

    printf("Database initialized: %s\n", DATABASE_FILE);
    
    char *schema = read_file("schema.sql");
    
    if (schema == NULL) {
        sqlite3_close(db);
        db = NULL;
        return -1;
    }

    char *errmsg = NULL;

    if (sqlite3_exec(db, schema, NULL, NULL, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "Schema initialization failed: %s\n", errmsg);

        sqlite3_free(errmsg);
        free(schema);

        sqlite3_close(db);
        db = NULL;

        return -1;
    }

    free(schema);

    printf("Database schema loaded successfully.\n");

    return 0;
}

void database_close(void){
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;

        printf("Database connection closed.\n");
    }
}
