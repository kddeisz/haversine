#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <math.h>

#define DEG2RAD(degrees) (degrees * M_PI / 180.0)
#define EARTH_RADIUS 6378.137

typedef struct Results {
  int id;
  double distance;
} Result;

sqlite3 *database;

int find_closest_geofence(double latitude, double longitude, Result *result) {
  char *sql = "SELECT id, haversine(?, ?, latitude, longitude) - (radius / 1000.0) AS dist \
    FROM geofences ORDER BY dist ASC LIMIT 1;";
  sqlite3_stmt *statement;

  if ((sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 1, latitude) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 2, longitude) != SQLITE_OK) ||
    (sqlite3_step(statement) != SQLITE_ROW)) {
    return 1;
  }

  result->id = sqlite3_column_int(statement, 0);
  result->distance = sqlite3_column_double(statement, 1);

  sqlite3_finalize(statement);
  return 0;
}

/** http://www.movable-type.co.uk/scripts/latlong.html */
void haversine(sqlite3_context *context, int argc, sqlite3_value *argv[]) {
  double lat1 = sqlite3_value_double(argv[0]);
  double lon1 = sqlite3_value_double(argv[1]);
  double lat2 = sqlite3_value_double(argv[2]);
  double lon2 = sqlite3_value_double(argv[3]);

  double φ1 = DEG2RAD(lat1);
  double φ2 = DEG2RAD(lat2);
  double Δφ = DEG2RAD(lat2 - lat1);
  double Δλ = DEG2RAD(lon2 - lon1);

  double a = sin(Δφ / 2) * sin(Δφ / 2) + cos(φ1) * cos(φ2) * sin(Δλ / 2) * sin(Δλ / 2);
  sqlite3_result_double(context, EARTH_RADIUS * 2 * atan2(sqrt(a), sqrt(1 - a)));
}

int main(int argc, char* argv[]) {
  int return_code;

  if ((return_code = sqlite3_open("test.sqlite3", &database))) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  }

  if ((return_code = sqlite3_create_function(database, "haversine", 4, SQLITE_UTF8, NULL, &haversine, NULL, NULL))) {
    fprintf(stderr, "Could not create haversine function: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  }

  Result *result = malloc(sizeof(Result));
  if ((return_code = find_closest_geofence(-70, 42.01000, result))) {
    fprintf(stderr, "Could not find closest geofence: %s\n", sqlite3_errmsg(database));
    free(result);
    exit(return_code);
  }

  printf("Found nearest geofence: {id=%d distance=%f}\n", result->id, result->distance);
  free(result);

  sqlite3_close(database);
}
