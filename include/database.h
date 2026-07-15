#pragma once

// Initializes the SQLite database connection (Creates it if it dosen't exist)
int database_init(void);

//Closes the current database connection.
void database_close(void);
