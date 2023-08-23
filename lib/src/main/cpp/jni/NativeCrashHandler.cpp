#include <jni.h>
#include <exception>
#include "android/log.h"
#include "../android/AndroidNativeCrashHandler.h"
#include "../bridge/nativeCrashExceptionJni.h"


void callCrashHandlerJNIOnNativeCrash(
        jobject nativeException,
        JNIEnv *env,
        jobject crashHandlerJNI
);


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_csense_android_crashhandler_CrashHandlerJNI_install(
        JNIEnv *env,
        jobject thiz
) {
    JNICrashHandlerCallback handler = [](JNIEnv *env, siginfo *info,
                                         jweak crashHandlerJNIRef) -> void {
        const char *message = createCrashMessage(info);
        jobject nativeException = newNativeCrashExceptionException(env, message, info);
        callCrashHandlerJNIOnNativeCrash(nativeException, env, crashHandlerJNIRef);
    };

    AndroidNativeCrashHandler *instance = AndroidNativeCrashHandler::getInstance();
    return instance->install(
            JniCrashHandler{
                    env,
                    handler,
                    thiz
            }
    );
}

void callCrashHandlerJNIOnNativeCrash(
        jobject nativeException,
        JNIEnv *env,
        jweak crashHandlerJNI
) {
    jclass clz = env->FindClass("com/csense/android/crashhandler/CrashHandlerJNI");
    jmethodID method = env->GetMethodID(clz, "onNativeCrash",
                                        "(Lcom/csense/android/crashhandler/NativeCrashException;)V");
    env->CallVoidMethod(crashHandlerJNI, method, nativeException);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_csense_android_crashhandler_CrashHandlerJNI_isInstalled(
        JNIEnv *env,
        jobject thiz
) {
    return AndroidNativeCrashHandler::getInstance()->isInstalled();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_csense_android_crashhandler_CrashHandlerJNI_uninstall(
        JNIEnv *env,
        jobject thiz
) {
    return AndroidNativeCrashHandler::getInstance()->uninstall();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_csense_android_crashhandler_CrashHandlerJNI_crashAndGetExceptionMessage(
        JNIEnv *env,
        jobject thiz
) {
    throw std::bad_exception();
}



