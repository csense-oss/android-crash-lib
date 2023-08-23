#include "jni.h"
#include <csignal>

#ifndef NATIVECRASHEXCEPTIONJNI_H
#define NATIVECRASHEXCEPTIONJNI_H


jobject newNativeCrashExceptionException(
        JNIEnv *env,
        const char *message,
        struct siginfo *info
);


#endif //NATIVECRASHEXCEPTIONJNI_H
