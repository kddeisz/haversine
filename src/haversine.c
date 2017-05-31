#include <stdlib.h>
#include <math.h>
#include "haversine.h"

#define DEG2RAD(degrees) (degrees * M_PI / 180.0)
#define EARTH_RADIUS 6378.137

double haversine(double lat1, double lon1, double lat2, double lon2) {
  double φ1 = DEG2RAD(lat1);
  double φ2 = DEG2RAD(lat2);
  double Δφ = DEG2RAD(lat2 - lat1);
  double Δλ = DEG2RAD(lon2 - lon1);

  double a = sin(Δφ / 2) * sin(Δφ / 2) + cos(φ1) * cos(φ2) * sin(Δλ / 2) * sin(Δλ / 2);
  return EARTH_RADIUS * 2 * atan2(sqrt(a), sqrt(1 - a));
}

void haversine_sqlite(sqlite3_context *context, int argc, sqlite3_value *argv[]) {
  sqlite3_result_double(context, haversine(
    sqlite3_value_double(argv[0]),
    sqlite3_value_double(argv[1]),
    sqlite3_value_double(argv[2]),
    sqlite3_value_double(argv[3])
  ));
}

int haversine_attach(sqlite3 *database) {
  return sqlite3_create_function(database, "haversine", 4, SQLITE_UTF8, NULL,
    &haversine_sqlite, NULL, NULL);
}
