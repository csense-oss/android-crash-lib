#include <jni.h>

#ifndef ANDROIDNATIVECRASHHANDLER_H
#define ANDROIDNATIVECRASHHANDLER_H

#include <csignal>
#include <array>
#include "../generic/generic.h"
#include "../bridge/jniCrashHandler.h"


class AndroidNativeCrashHandler {

public:

    AndroidNativeCrashHandler(AndroidNativeCrashHandler const &) = delete;

    AndroidNativeCrashHandler &operator=(AndroidNativeCrashHandler const &) = delete;

    ~AndroidNativeCrashHandler() {}

    bool install(JniCrashHandler handler);

    bool uninstall();

    bool isInstalled() const;

    static AndroidNativeCrashHandler *getInstance() {
        static AndroidNativeCrashHandler instance{};
        return &instance;
    }

private:

    struct JniCrashHandler nativeCrashHandler = JniCrashHandler();

    constexpr static const std::array SIGNALS_TO_CATCH = {
            SIGABRT,
            SIGBUS,
            SIGFPE,
            SIGSEGV,
            SIGILL,
            SIGSTKFLT,
            SIGTRAP,
    };

    struct sigaction overwrittenSignals[NSIG];

    explicit AndroidNativeCrashHandler() {}

    bool tryInstallNativeHandlers();

    static struct sigaction createNativeActionHandler();

    void nativeCrashSignalHandler(siginfo *siginfo);

    void callCallbackHandlerWithCrash(siginfo *siginfo);

    static void resendSignalToAndroidHandlerIfNeeded(siginfo *siginfo);
};


#endif //ANDROIDNATIVECRASHHANDLER_H
