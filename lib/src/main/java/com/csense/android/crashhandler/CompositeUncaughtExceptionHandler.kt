package com.csense.android.crashhandler

class CompositeUncaughtExceptionHandler : Thread.UncaughtExceptionHandler {
    private val toDelegateTo: List<Thread.UncaughtExceptionHandler>

    constructor(toDelegateTo: List<Thread.UncaughtExceptionHandler>) {
        this.toDelegateTo = toDelegateTo
    }

    constructor(vararg toDelegateTo: Thread.UncaughtExceptionHandler) {
        this.toDelegateTo = toDelegateTo.toList()
    }

    override fun uncaughtException(thread: Thread, exception: Throwable) {
        toDelegateTo.forEach { handler: Thread.UncaughtExceptionHandler ->
            handler.uncaughtException(thread, exception)
        }
    }

}