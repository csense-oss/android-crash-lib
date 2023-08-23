#include <jni.h>
#include <csignal>

#ifndef JNICRASHHANDLER_H
#define JNICRASHHANDLER_H

typedef void (*JNICrashHandlerCallback)(JNIEnv *env, siginfo *info, jweak crashHandlerJNIRef);

struct JniCrashHandler {

public:

    JniCrashHandler();

    JniCrashHandler(
            JNIEnv *newEnv,
            JNICrashHandlerCallback newHandler,
            jobject newCrashHandlerJNI
    );

    void install(JniCrashHandler newHandler);

    bool isSet() const;

    void uninstall();

    void operator()(siginfo *info) const;

private:

    void setFields(
            JNIEnv *newEnv,
            JNICrashHandlerCallback newHandler,
            jweak newCrashHandlerJNIRef
    );

    JNIEnv *env = nullptr;
    JNICrashHandlerCallback handler = nullptr;
    jweak crashHandlerJNIRef = nullptr;

    void removeGlobalRef();
};


#endif //JNICRASHHANDLER_H
