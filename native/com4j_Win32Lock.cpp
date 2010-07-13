#include "stdafx.h"
#include "com4j.h"
#include "com4j_Win32Lock.h"

JNIEXPORT void JNICALL Java_com4j_Win32Lock_closeHandle( JNIEnv* env, jclass _, jint handle ) {
	::CloseHandle(reinterpret_cast<HANDLE>(handle));
}

JNIEXPORT jint JNICALL Java_com4j_Win32Lock_createEvent( JNIEnv* env , jclass _) {
	HANDLE h = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	return reinterpret_cast<jint>(h);
}

JNIEXPORT void JNICALL Java_com4j_Win32Lock_activate0( JNIEnv* env, jclass _, jint handle ) {
	::SetEvent(reinterpret_cast<HANDLE>(handle));	
}

void suspend00(JNIEnv* env, HANDLE h, DWORD timeoutMillis) {
	while(true) {
		MSG msg;
		DWORD r = ::MsgWaitForMultipleObjects(1, &h, FALSE, timeoutMillis, QS_ALLINPUT);
		switch(r) {
		case WAIT_OBJECT_0:
			// event signaled
		case WAIT_TIMEOUT:
			// timeout elapsed
			return;
		case WAIT_OBJECT_0 +1:
			// message arrived
			while(PeekMessage(&msg,NULL,0,0,TRUE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		default:
			error(env,__FILE__,__LINE__,"unexpected return value from MsgWaitForMultipleObjects: %d",r);
			return;
		}
	}
}

JNIEXPORT void JNICALL Java_com4j_Win32Lock_suspend0(JNIEnv* env, jclass _, jint handle) {
	HANDLE h = reinterpret_cast<HANDLE>(handle);
	suspend00(env, h, INFINITE);
}

JNIEXPORT void JNICALL Java_com4j_Win32Lock_suspend1(JNIEnv* env, jclass _, jint handle, jint timeoutMillis) {
	HANDLE h = reinterpret_cast<HANDLE>(handle);
	suspend00(env, h, timeoutMillis);
}
