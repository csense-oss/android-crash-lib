package com.csense.android.crashhandler

import java.lang.Thread.*

class JavaCrashHandler {

    private var lastHandler: UncaughtExceptionHandler? = null
    fun install(newHandler: UncaughtExceptionHandler) {
        val current: Thread = currentThread()
        lastHandler = current.uncaughtExceptionHandler
        current.uncaughtExceptionHandler = CompositeUncaughtExceptionHandler(
            listOfNotNull(newHandler, lastHandler)
        )
    }


    fun uninstall() {
        currentThread().uncaughtExceptionHandler = lastHandler
    }
}

