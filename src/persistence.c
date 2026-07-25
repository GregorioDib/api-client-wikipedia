#include <stdio.h>

#include <sqlite3.h>

#include "config.h"
#include "database.h"
#include "persistence.h"

// Stores a Wikipedia article into the assets table.
long long persistence_save_article(const wiki_article_t *article){
    if (article == NULL){
        return -1;}

    sqlite3 *db = database_get_connection();

    if (db == NULL){
        return -1;}

    static const char *sql =
        "INSERT INTO assets "
        "(asset_uri, title, entity, provider, meta_payload) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *statement = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK){
        fprintf(stderr, "Unable to prepare SQL statement.\n");

        return -1;
    }

    sqlite3_bind_text(statement, 1, article->asset_uri, -1, SQLITE_STATIC);

    sqlite3_bind_text(statement, 2, article->title, -1, SQLITE_STATIC);

    sqlite3_bind_text(statement, 3, "article", -1, SQLITE_STATIC);

    sqlite3_bind_text(statement, 4, PROVIDER_NAME, -1, SQLITE_STATIC);

    sqlite3_bind_text(statement, 5, article->meta_payload, -1, SQLITE_STATIC);

    if (sqlite3_step(statement) != SQLITE_DONE){
        fprintf(stderr, "Unable to store article: %s\n", sqlite3_errmsg(db));

        sqlite3_finalize(statement);

        return -1;
    }

    sqlite3_finalize(statement);

    return sqlite3_last_insert_rowid(db);
}
