#ifndef HAVERSINE
#define HAVERSINE

#include <sqlite3.h>

/**
 * Attach a "haversine" UDF to the given database connection. The function is
 * based on the algorithm described here:
 * http://www.movable-type.co.uk/scripts/latlong.html. The return value is the
 * return code from the sqlite3_create_function call.
 */
int haversine_attach(sqlite3 *database);

#endif
