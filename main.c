#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3 *database;

typedef struct Geofences {
  int id;
  double lat;
  double lng;
  double rad;
} Geofence;

int find_closest_geofence(double cur_lat, double cur_lng, Geofence *geofence) {
  char *sql = "SELECT id, lat, lng, rad FROM geofences ORDER BY geodist(?, ?, lat, lng, rad) LIMIT 1;";
  sqlite3_stmt *statement;

  if ((sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 1, cur_lat) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 2, cur_lng) != SQLITE_OK)) {
    printf("GOT HERE!\n");
    return 1;
  }

  if (sqlite3_step(statement) != SQLITE_ROW) {
    return 1;
  }

  geofence->id = sqlite3_column_int(statement, 0);
  geofence->lat = sqlite3_column_double(statement, 1);
  geofence->lng = sqlite3_column_double(statement, 2);
  geofence->rad = sqlite3_column_double(statement, 3);

  sqlite3_finalize(statement);
  return 0;
}

void geodist(sqlite3_context *context, int argc, sqlite3_value *argv[]) {
  double cur_lat = sqlite3_value_double(argv[0]);
  double cur_lng = sqlite3_value_double(argv[1]);
  double lat = sqlite3_value_double(argv[2]);
  double lng = sqlite3_value_double(argv[3]);
  double rad = sqlite3_value_double(argv[4]);

  double dist = (cur_lat - lat) * (cur_lat - lat) + (cur_lng - lng) * (cur_lng - lng);
  sqlite3_result_double(context, dist);
}

int add_geodist_function() {
  return sqlite3_create_function(database, "geodist", 5, SQLITE_UTF8, NULL, &geodist, NULL, NULL);
}

int insert_geofence(int id, double lat, double lng, double rad) {
  char *sql = "INSERT INTO geofences (id, lat, lng, rad) VALUES (?, ?, ?, ?)";
  sqlite3_stmt *statement;

  if ((sqlite3_prepare_v2(database, sql, -1, &statement, NULL) != SQLITE_OK) ||
    (sqlite3_bind_int(statement, 1, id) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 2, lat) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 3, lng) != SQLITE_OK) ||
    (sqlite3_bind_double(statement, 4, rad) != SQLITE_OK) ||
    (sqlite3_step(statement) != SQLITE_DONE)) {

    return 1;
  }

  sqlite3_finalize(statement);
  return 0;
}

int create_table() {
  char *sql = "CREATE TABLE IF NOT EXISTS 'geofences' ('id' INTEGER PRIMARY KEY, \
    'lat' INTEGER NOT NULL, 'lng' INTEGER NOT NULL, 'rad' INTEGER NOT NULL);";

  if (sqlite3_exec(database, sql, NULL, NULL, NULL) != SQLITE_OK) {
    sqlite3_close(database);
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int return_code = sqlite3_open("test.db", &database);

  if (return_code) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  return_code = create_table();
  if (return_code) {
    fprintf(stderr, "Could not create the geofences table: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  } else {
    fprintf(stderr, "Created geofences table\n");
  }

  return_code = insert_geofence(1, -71.05910, 42.35906, 300);
  if (return_code) {
    fprintf(stderr, "Could not insert first geofence: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  } else {
    fprintf(stderr, "Inserted first geofence\n");
  }

  return_code = insert_geofence(2, -71.06080, 42.35937, 50);
  if (return_code) {
    fprintf(stderr, "Could not insert second geofence: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  } else {
    fprintf(stderr, "Inserted second geofence\n");
  }

  return_code = add_geodist_function();
  if (return_code) {
    fprintf(stderr, "Could not create geodist function: %s\n", sqlite3_errmsg(database));
    exit(return_code);
  } else {
    fprintf(stderr, "Created geodist function\n");
  }

  Geofence *geofence = malloc(sizeof(Geofence));
  return_code = find_closest_geofence(-71.063321, 42.359671, geofence);
  if (return_code) {
    fprintf(stderr, "Could not find closest geofence: %s\n", sqlite3_errmsg(database));
    free(geofence);
    exit(return_code);
  } else {
    fprintf(stderr, "Found nearest geofence: id %d\n", geofence->id);
  }
  free(geofence);

  sqlite3_close(database);
}
