#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include "config.h"
#include "database.h"
#include "persistence.h"

//------------------------------------------PRIVATE HELPER FUNCTIONS--------------------------------------------

// Duplicates a C string using dynamic memory.
static char *string_duplicate(const char *source){

    if (source == NULL){
    
        return NULL;
    }
    
    size_t length = strlen(source) + 1;

    char *copy = malloc(length);

    if (copy == NULL){
    
        return NULL;
    }
    
    memcpy(copy, source, length);

    return copy;
}

// Loads an existing asset from the database
static int load_existing_asset(const char *asset_uri, long long *asset_id, char **meta_payload){

    sqlite3 *db = database_get_connection();

    if (db == NULL){
    
        return -1;
    }

    static const char *sql =
        "SELECT id, meta_payload "
        "FROM assets "
        "WHERE asset_uri = ?;";

    sqlite3_stmt *statement = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK){
        fprintf(stderr, "Unable to prepare lookup statement.\n");

        return -1;
    }

    sqlite3_bind_text(statement, 1, asset_uri, -1, SQLITE_STATIC);

    int rc = sqlite3_step(statement);

    if (rc == SQLITE_ROW){
        *asset_id = sqlite3_column_int64(statement, 0);

    const unsigned char *payload = sqlite3_column_text(statement, 1);

    if (payload == NULL){
        sqlite3_finalize(statement);
        
        return -1;
    }

    *meta_payload = string_duplicate((const char *)payload);
    
    if (*meta_payload == NULL){
        sqlite3_finalize(statement);
        
        return -1;
    }

    sqlite3_finalize(statement);

    return 1;
}

    sqlite3_finalize(statement);

    if (rc == SQLITE_DONE){
    
        return 0;
    }

    fprintf(stderr, "Asset lookup failed: %s\n", sqlite3_errmsg(db));

    return -1;
}

// Stores the previous version of an asset into the history table.
static int insert_history_entry(long long asset_id, const char *history_payload){
    sqlite3 *db = database_get_connection();

    if (db == NULL){
        return -1;
    }

    static const char *sql =
        "INSERT INTO assets_history "
        "(asset_id, history_payload) "
        "VALUES (?, ?);";

    sqlite3_stmt *statement = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK){
        fprintf(stderr, "Unable to prepare history statement.\n");

        return -1;
    }

    sqlite3_bind_int64(statement, 1, asset_id);

    sqlite3_bind_text(statement, 2, history_payload, -1, SQLITE_STATIC);

    if (sqlite3_step(statement) != SQLITE_DONE){
    
        fprintf(stderr, "Unable to insert history entry: %s\n", sqlite3_errmsg(db));

        sqlite3_finalize(statement);

        return -1;
    }

    sqlite3_finalize(statement);

    return 0;
}

//Updates an existing asset with newly retrieved metadata.
static int update_existing_asset(long long asset_id, const wiki_article_t *article){

    sqlite3 *db = database_get_connection();

    if (db == NULL){
        return -1;
    }

    static const char *sql =
        "UPDATE assets "
        "SET title = ?, "
        "meta_payload = ?, "
        "updated_at = CURRENT_TIMESTAMP "
        "WHERE id = ?;";

    sqlite3_stmt *statement = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK){
        fprintf(stderr, "Unable to prepare update statement.\n");

        return -1;
    }

    sqlite3_bind_text(statement, 1, article->title, -1, SQLITE_STATIC);

    sqlite3_bind_text(statement, 2, article->meta_payload, -1, SQLITE_STATIC);

    sqlite3_bind_int64(statement, 3, asset_id);

    if (sqlite3_step(statement) != SQLITE_DONE){
        fprintf(stderr, "Unable to update asset: %s\n", sqlite3_errmsg(db));

        sqlite3_finalize(statement);

        return -1;
    }

    sqlite3_finalize(statement);

    return 0;
}

//------------------------------------------PUBLIC INTERFACE--------------------------------------------

// Persists a Wikipedia article into the database
persistence_result_t persistence_save_article(const wiki_article_t *article, long long *asset_id){

    if (article == NULL || asset_id == NULL){
    
        return PERSISTENCE_ERROR;
    }

    sqlite3 *db = database_get_connection();

    if (db == NULL){
    
        return PERSISTENCE_ERROR;
    }

    long long existing_id = 0;
    char *existing_payload = NULL;

    int exists = load_existing_asset(article->asset_uri, &existing_id, &existing_payload);

    if (exists == -1){
    
        return PERSISTENCE_ERROR;
    }

    // Existing asset
    if (exists == 1) {

        // Nothing changed
        if (strcmp(existing_payload, article->meta_payload) == 0){
        
            free(existing_payload);

            *asset_id = existing_id;

            return PERSISTENCE_UNCHANGED;
        }

        // Preserve the previous version
        if (insert_history_entry(existing_id, existing_payload) != 0){
        
            free(existing_payload);
            
            return PERSISTENCE_ERROR;
        }

        free(existing_payload);

        // Update current asset
        if (update_existing_asset(existing_id, article) != 0){
        
            return PERSISTENCE_ERROR;
        }

        *asset_id = existing_id;

        return PERSISTENCE_UPDATED;
    }

    // If the asset does not exist -> insert

    static const char *sql =
        "INSERT INTO assets "
        "(asset_uri, title, entity, provider, meta_payload) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *statement = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK){
    
        fprintf(stderr, "Unable to prepare SQL statement.\n");

        return PERSISTENCE_ERROR;
    }

    sqlite3_bind_text(statement, 1, article->asset_uri, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, article->title, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, "article", -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, PROVIDER_NAME, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 5, article->meta_payload, -1, SQLITE_STATIC);

    if (sqlite3_step(statement) != SQLITE_DONE){

        fprintf(stderr, "Unable to store article: %s\n", sqlite3_errmsg(db));

        sqlite3_finalize(statement);

        return PERSISTENCE_ERROR;
    }

    sqlite3_finalize(statement);

    *asset_id = sqlite3_last_insert_rowid(db);

    return PERSISTENCE_INSERTED;
}

