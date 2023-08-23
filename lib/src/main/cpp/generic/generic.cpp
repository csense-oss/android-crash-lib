#include "generic.h"

#include <iostream>
#include <vector>

const char *createCrashMessageFrom(std::type_info *info, void *current_exception);

int removeSigaction(int signal, const struct sigaction *action) {
    return sigaction(
            /* signal= */       signal,
            /* __new_action= */ action,
            /* __old_action= */ nullptr
    );
}

///currently based on https://testfairy.com/blog/ndk-crash-handling/
/// Create a crash message using whatever available such as signal, C++ exception etc
const char *createCrashMessage(siginfo *siginfo) {
    void *current_exception = __cxxabiv1::__cxa_current_primary_exception();
    std::type_info *current_exception_type_info = __cxxabiv1::__cxa_current_exception_type();

    size_t buffer_size = 1024;
    char *abort_message = static_cast<char *>(malloc(buffer_size));

    if (current_exception) {
        try {
            // Check if we can get the message
            if (current_exception_type_info) {
                return createCrashMessageFrom(
                        current_exception_type_info,
                        current_exception
                );
            }
        } catch (std::bad_cast &bc) {

        }
    }
    // Not a cpp exception, assume a custom crash and act like C
    // Assume C crash and print signal no and code
    sprintf(abort_message, "Terminating with a C crash %d : %d", siginfo->si_signo,
            siginfo->si_code);
    return abort_message;
}


const char *createCrashMessageFrom(std::type_info *info, void *current_exception) {

    size_t buffer_size = 1024;
    char *abort_message = static_cast<char *>(malloc(buffer_size));

    const char *exception_name = info->name();

    int demangleStatus = -1;
    char demangled_name[buffer_size];
    __cxxabiv1::__cxa_demangle(exception_name, demangled_name, &buffer_size, &demangleStatus);

    if (demangleStatus) {
        // Couldn't demangle, go with exception_name
        sprintf(abort_message, "Terminating with uncaught exception of type %s",
                exception_name);
        return abort_message;
    }

    if (strstr(demangled_name, "nullptr") || strstr(demangled_name, "NULL")) {
        // Could demangle, go with demangled_name and state that it was null
        sprintf(abort_message, "Terminating with uncaught exception of type %s",
                demangled_name);
        return abort_message;
    }

    // Could demangle, go with demangled_name and exception.what() if exists
    try {
        __cxxabiv1::__cxa_rethrow_primary_exception(current_exception);
    } catch (std::exception &e) {
        // Include message from what() in the abort message
        sprintf(abort_message,
                "Terminating with uncaught exception of type %s : %s",
                demangled_name, e.what());
    } catch (...) {
        // Just report the exception type since it is not an std::exception
        sprintf(abort_message, "Terminating with uncaught exception of type %s",
                demangled_name);
    }
    return abort_message;
}