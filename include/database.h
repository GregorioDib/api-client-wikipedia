#pragma once

#include <sqlite3.h>

// Initializes the SQLite database connection
int database_init(void);

// Returns pointer to the active SQLite connection
sqlite3 *database_get_connection(void);

// Closes the database connection
void database_close(void);
