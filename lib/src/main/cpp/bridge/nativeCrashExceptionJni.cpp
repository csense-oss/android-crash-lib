#include "nativeCrashExceptionJni.h"

jobject newNativeCrashExceptionException(
        JNIEnv *env,
        const char *message,
        struct siginfo *info
) {
    jclass cls = env->FindClass("com/csense/android/crashhandler/NativeCrashException");
    jmethodID constructor = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;III)V");
    return env->NewObject(
            /*clazz=*/cls,
            /*methodID=*/constructor,
                      env->NewStringUTF(message),
                      info->si_signo,
                      info->si_errno,
                      info->si_code
    );
}