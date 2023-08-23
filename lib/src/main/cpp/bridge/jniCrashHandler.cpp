#include "jniCrashHandler.h"

JniCrashHandler::JniCrashHandler(
        JNIEnv *newEnv,
        JNICrashHandlerCallback newHandler,
        jobject newCrashHandlerJNI
) {
    jweak ref = newEnv->NewWeakGlobalRef(newCrashHandlerJNI);
    setFields(newEnv, newHandler, ref);

}

JniCrashHandler::JniCrashHandler() = default;


void JniCrashHandler::uninstall() {
    if (!isSet()) {
        return;
    }
    removeGlobalRef();
    env = nullptr;
    handler = nullptr;
    crashHandlerJNIRef = nullptr;
}

bool JniCrashHandler::isSet() const {
    return env != nullptr && handler != nullptr && crashHandlerJNIRef != nullptr;
}

void JniCrashHandler::operator()(siginfo *info) const {
    handler(env, info, crashHandlerJNIRef);
}


void JniCrashHandler::install(JniCrashHandler newHandler) {
    setFields(
            /*newEnv=*/newHandler.env,
            /*newHandler=*/newHandler.handler,
            /*newCrashHandlerJNIRef=*/newHandler.crashHandlerJNIRef
    );
}


void JniCrashHandler::setFields(
        JNIEnv *newEnv,
        JNICrashHandlerCallback newHandler,
        jweak newCrashHandlerJNIRef
) {
    env = newEnv;
    handler = newHandler;
    crashHandlerJNIRef = newCrashHandlerJNIRef;
}

void JniCrashHandler::removeGlobalRef() {
    env->DeleteWeakGlobalRef(crashHandlerJNIRef);
}
