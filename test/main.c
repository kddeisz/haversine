#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../src/haversine.h"

typedef struct Results {
  int id;
  double distance;
} Result;

sqlite3 *database;

static int find_closest_geofence(double latitude, double longitude, Result *result) {
  char *sql = "SELECT id, haversine(?, ?, latitude, longitude) - radius AS dist \
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

int main(int argc, char* argv[]) {
  int return_code;

  if ((return_code = sqlite3_open("test/test.sqlite3", &database))) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  }

  if ((return_code = haversine_attach(database))) {
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
