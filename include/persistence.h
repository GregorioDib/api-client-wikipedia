#pragma once

#include "json.h"

typedef enum{
    PERSISTENCE_ERROR = 0,
    PERSISTENCE_INSERTED,
    PERSISTENCE_UPDATED,
    PERSISTENCE_UNCHANGED
} persistence_result_t;

// Persists a Wikipedia article into the assets table
persistence_result_t persistence_save_article(const wiki_article_t *article, long long *asset_id);
