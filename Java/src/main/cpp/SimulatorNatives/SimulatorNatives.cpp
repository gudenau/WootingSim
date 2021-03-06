#include <windows.h>

#include <jni.h>

#include <net_gudenau_wooting_simulator_Native.h>

#define null nullptr
#define UNUSED(var) {(var) = (var);}

JNIEXPORT jlong JNICALL Java_net_gudenau_wooting_simulator_Native_CreateNamedPipe
(JNIEnv* env, jclass klass, jstring lpNameJava, jint dwOpenMode, jint dwPipeMode, jint nMaxInstances, jint nOutBufferSize, jint nInBufferSize, jint nDefaultTimeOut) {
	UNUSED(klass);

	const jchar* lpName = env->GetStringChars(lpNameJava, null);

	HANDLE handle = CreateNamedPipe(
		(LPCWSTR) lpName,
		(DWORD)dwOpenMode,
			(DWORD)dwPipeMode,
			(DWORD)nMaxInstances,
				(DWORD)nOutBufferSize,
				(DWORD)nInBufferSize,
					(DWORD)nDefaultTimeOut,
		null
	);

	env->ReleaseStringChars(lpNameJava, lpName);

	return (jlong)handle;
}

JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_ConnectNamedPipe
(JNIEnv* env, jclass klass, jlong handle) {
	UNUSED(env);
	UNUSED(klass);

	return (jboolean)ConnectNamedPipe(
		(HANDLE)handle,
		null
	);
}

JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_ReadFile
(JNIEnv* env, jclass klass, jlong hFile, jlong lpBuffer, jint nNumberOfBytesToRead, jlong lpNumberOfBytesRead) {
	UNUSED(env);
	UNUSED(klass);

	return (jboolean)ReadFile(
		(HANDLE)hFile,
		(LPVOID)lpBuffer,
		(DWORD)nNumberOfBytesToRead,
		(LPDWORD)lpNumberOfBytesRead,
		null
	);
}

JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_WriteFile
(JNIEnv* env, jclass klass, jlong hFile, jlong lpBuffer, jint nNumberOfBytesToWrite, jlong lpNumberOfBytesWritten) {
	UNUSED(env);
	UNUSED(klass);

	return (jboolean)WriteFile(
		(HANDLE)hFile,
		(LPCVOID)lpBuffer,
		(DWORD)nNumberOfBytesToWrite,
		(LPDWORD)lpNumberOfBytesWritten,
		null
	);
}

JNIEXPORT jboolean JNICALL Java_net_gudenau_wooting_simulator_Native_CloseHandle
(JNIEnv* env, jclass klass, jlong hObject) {
	UNUSED(env);
	UNUSED(klass);

	return (jboolean)CloseHandle(
		(HANDLE)hObject
	);
}

JNIEXPORT jint JNICALL Java_net_gudenau_wooting_simulator_Native_GetLastError
(JNIEnv* env, jclass klass) {
	UNUSED(env);
	UNUSED(klass);

	return (jint)GetLastError();
}

JNIEXPORT jlong JNICALL Java_net_gudenau_wooting_simulator_Native_getByteBufferAddress
(JNIEnv* env, jclass klass, jobject buffer) {
	UNUSED(klass);
	
	return (jlong)env->GetDirectBufferAddress(buffer);
}