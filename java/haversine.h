#include <jni.h>

#ifndef _Included_Haversine
#define _Included_Haversine

/*
 * Class:     Haversine
 * Method:    haversine
 * Signature: (DDDD)D
 */
JNIEXPORT jdouble JNICALL Java_Haversine_haversine
  (JNIEnv *, jobject, jdouble, jdouble, jdouble, jdouble);

#endif
