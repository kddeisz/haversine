CREATE TABLE IF NOT EXISTS 'geofences' (
  'id' INTEGER PRIMARY KEY AUTOINCREMENT,
  'latitude' INTEGER NOT NULL,
  'longitude' INTEGER NOT NULL,
  'radius' INTEGER NOT NULL
);

INSERT INTO geofences (latitude, longitude, radius)
  VALUES (-70, 42.00000, 0.30);

INSERT INTO geofences (latitude, longitude, radius)
  VALUES (-70, 42.00050, 0.05);
