package com.csense.android.crashhandler

class CrashHandlerJNI(
    private val onNativeCrashHandler: (t: NativeCrashException) -> Unit
) {

    /**
     * installs native signal handler to catch native crashes. returns false if already installed
     */
    external fun install(

    ): Boolean

    external fun isInstalled(): Boolean

    /**
     * uninstalls the native signal handler(s)
     */
    external fun uninstall(): Boolean


    //TODO DEBUG hmmm...
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application. It will throw a C++ exception
     * and catch it in the signal handler which will be visible in the logs.
     */
    external fun crashAndGetExceptionMessage()

    fun onNativeCrash(nativeException: NativeCrashException) {
        onNativeCrashHandler(nativeException)
    }

    init {
        System.loadLibrary("native-crash-handler")
    }

}