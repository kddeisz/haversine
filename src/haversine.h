#ifndef HAVERSINE
#define HAVERSINE

#include <sqlite3.h>

/**
 * Compute the haversine distance based on the algorithm describe here:
 * http://www.movable-type.co.uk/scripts/latlong.html.
 */
double haversine(double lat1, double lon1, double lat2, double lon2);

/**
 * Attach a "haversine" UDF to the given database connection. The return value
 * is the return code from the sqlite3_create_function call.
 */
int haversine_attach(sqlite3 *database);

#endif
