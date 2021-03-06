/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class net_gudenau_wooting_simulator_Native */

#ifndef _Included_net_gudenau_wooting_simulator_Native
#define _Included_net_gudenau_wooting_simulator_Native
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    CreateNamedPipe
 * Signature: (Ljava/lang/String;IIIIII)J
 */
JNIEXPORT jlong JNICALL Java_net_gudenau_wooting_simulator_Native_CreateNamedPipe
  (JNIEnv *, jclass, jstring, jint, jint, jint, jint, jint, jint);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    ConnectNamedPipe
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_ConnectNamedPipe
  (JNIEnv *, jclass, jlong);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    ReadFile
 * Signature: (JJIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_ReadFile
  (JNIEnv *, jclass, jlong, jlong, jint, jlong);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    WriteFile
 * Signature: (JJIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_WriteFile
  (JNIEnv *, jclass, jlong, jlong, jint, jlong);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    CloseHandle
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_CloseHandle
  (JNIEnv *, jclass, jlong);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    GetLastError
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gudenau_wooting_simulator_Native_GetLastError
  (JNIEnv *, jclass);

/*
 * Class:     net_gudenau_wooting_simulator_Native
 * Method:    getByteBufferAddress
 * Signature: (Ljava/nio/ByteBuffer;)J
 */
JNIEXPORT jlong JNICALL Java_net_gudenau_wooting_simulator_Native_getByteBufferAddress
  (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif
