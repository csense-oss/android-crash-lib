#include "AndroidNativeCrashHandler.h"
#include <algorithm>
#include <array>
#include <android/log.h>
#include <unistd.h>

bool AndroidNativeCrashHandler::install(
        JniCrashHandler handler
) {
    if (isInstalled()) {
        return false;
    }

    bool didInstall = tryInstallNativeHandlers();
    if (!didInstall) {
        uninstall();
    }
    nativeCrashHandler.install(handler);
    return didInstall;
}

bool AndroidNativeCrashHandler::uninstall() {
    if (!isInstalled()) {
        return false;
    }

    for_each_indexed_n(
            /* first=*/ overwrittenSignals,
            /* orig_n=*/ std::size(overwrittenSignals),
            /* action= */ [](int index, struct sigaction sig) -> void {
                if (sig.sa_handler == nullptr) {
                    return;
                }
                removeSigaction(
                        /*signal=*/ index,
                        /*action=*/ &sig
                );
            });
    nativeCrashHandler.uninstall();
    return true;
}

bool AndroidNativeCrashHandler::isInstalled() const {
    return nativeCrashHandler.isSet();
}

bool AndroidNativeCrashHandler::tryInstallNativeHandlers() {

    struct sigaction newSig = createNativeActionHandler();

    return std::all_of(
            SIGNALS_TO_CATCH.begin(),
            SIGNALS_TO_CATCH.end(),
            [newSig, this](int signal) -> int {
                return sigaction(
                        /*signal = */     signal,
                        /*__new_action=*/ &newSig,
                        /*__old_action=*/ &overwrittenSignals[signal]
                ) == 0;
            });
}

struct sigaction AndroidNativeCrashHandler::createNativeActionHandler() {
    struct sigaction sigactionstruct{};
    sigactionstruct.sa_flags = SA_SIGINFO;
    sigactionstruct.sa_sigaction = [](int signalNumber, siginfo *info, void *ctx) -> void {
        AndroidNativeCrashHandler *instance = AndroidNativeCrashHandler::getInstance();
        instance->nativeCrashSignalHandler(/* siginfo= */info);
    };
    return sigactionstruct;
}


void AndroidNativeCrashHandler::nativeCrashSignalHandler(
        siginfo *siginfo
) {
    callCallbackHandlerWithCrash(siginfo);
    uninstall();
    resendSignalToAndroidHandlerIfNeeded(siginfo);
}

void AndroidNativeCrashHandler::callCallbackHandlerWithCrash(
        siginfo *siginfo
) {
    if (!nativeCrashHandler.isSet()) {
        __android_log_print(ANDROID_LOG_ERROR, "AndroidNativeCrashHandler", "%s",
                            "Native crash handler called with no callback, programmer error.");
        return;
    }
    nativeCrashHandler(siginfo);
}

void AndroidNativeCrashHandler::resendSignalToAndroidHandlerIfNeeded(
        siginfo *siginfo
) {
    bool shouldRerunAndroidHandlers = SI_FROMUSER(siginfo) || siginfo->si_signo == SIGABRT;
    if (!shouldRerunAndroidHandlers) {
        return;
    }
    bool didKill = tgkill(
            /* __tgid= */  getpid(),
            /*__tid= */    gettid(),
            /*__signal= */ siginfo->si_signo
    ) == 0;

    if (!didKill) {
        _exit(/* __status= */ 1);
    }
}
