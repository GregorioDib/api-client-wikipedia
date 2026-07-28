#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "config.h"
#include "database.h"

// Active SQLite connection. Remains open for the app's lifetime and is closed before termination
static sqlite3 *db = NULL;

static char *read_file(const char *filename){

    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        perror("Unable to open schema file");
        
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (buffer == NULL){
        fclose(file);
        
        return NULL;
    }
    
    if (fread(buffer, 1, size, file) != (size_t)size){
        fclose(file);
        free(buffer);
        
        return NULL;
    }
    
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

int database_init(void){

    if (sqlite3_open(DATABASE_FILE, &db) != SQLITE_OK){
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));

        sqlite3_close(db);
        db = NULL;

        return -1;
    }
    
    char *schema = read_file("schema.sql");
    
    if (schema == NULL){
        sqlite3_close(db);
        db = NULL;
        
        return -1;
    }

    char *errmsg = NULL;

    if (sqlite3_exec(db, schema, NULL, NULL, &errmsg) != SQLITE_OK){
        fprintf(stderr, "Schema initialization failed: %s\n", errmsg);

        sqlite3_free(errmsg);
        free(schema);

        sqlite3_close(db);
        db = NULL;

        return -1;
    }

    free(schema);

    return 0;
}

sqlite3 *database_get_connection(void){

    return db;
}

void database_close(void){

    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}
