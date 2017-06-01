#include <stdio.h>
#include "haversine.h"
#include "../src/haversine.h"

JNIEXPORT jdouble JNICALL Java_Haversine_haversine(JNIEnv *env, jobject obj, jdouble lat1, jdouble lon1, jdouble lat2, jdouble lon2) {
  return haversine(lat1, lon1, lat2, lon2);
}
