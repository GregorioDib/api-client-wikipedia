#pragma once

#include <sqlite3.h>

// Initializes the SQLite database connection (Creates it, if it does not exist)
int database_init(void);

// Pointer to the active connection
sqlite3 *database_get_connection(void);

// Closes the database connection
void database_close(void);
