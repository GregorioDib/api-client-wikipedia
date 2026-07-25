#pragma once

#include "json.h"

/* Persists a Wikipedia article into the assets table. Returns:
 *   >= 0  SQLite row ID
 *   -1    Error */
long long persistence_save_article(const wiki_article_t *article);
