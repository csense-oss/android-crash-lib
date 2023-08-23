package com.csense.android.crashhandler

import android.util.*
import java.io.*

object CrashHandler {

    private val javaCrashHandler = JavaCrashHandler()

    private var storageLocation: File? = null

    private var log: Boolean = false


    private val jni: CrashHandlerJNI = CrashHandlerJNI { it: NativeCrashException ->
        onCrash(it)
    }
    fun crashNative() {
        jni.crashAndGetExceptionMessage()
    }


    fun install(
        log: Boolean = true,
        storageLocation: File
    ) {
        this.log = log
        val didInstallNative: Boolean = jni.install()
        if (didInstallNative) {
            javaCrashHandler.install { thread: Thread, exception: Throwable ->
                onCrash(exception)
            }
            this.storageLocation = storageLocation
        }
        if (!log) {
            return
        }
        didInstallNative.logInstallation()
    }

    private fun Boolean.logInstallation() {
        when (this) {
            true -> Log.i(
                /* tag = */ "CrashHandler",
                /* msg = */ "installed native crash handler"
            )

            false -> Log.e(
                /* tag = */ "CrashHandler",
                /* msg = */"failed to install native crash handler"
            )
        }
    }

    fun isInstalled(): Boolean {
        return jni.isInstalled()
    }

    fun uninstall() {
        jni.uninstall()
        javaCrashHandler.uninstall()
    }

    private fun onCrash(throwable: Throwable) {
        storeCrash(throwable)
        logCrash(throwable)
    }

    private fun logCrash(throwable: Throwable) {
        if (!log) {
            return
        }
        Log.e("callbacks", "crash", throwable)
    }

    private fun storeCrash(throwable: Throwable) {
        val crashfile: File = getCrashFileOrNull() ?: return
        crashfile.writeText(throwable.stackTraceToString())
    }


    fun haveCrashStored(): Boolean {
        return getCrashFileOrNull()?.exists() ?: false
    }

    fun tryReadCrash(onCrashFileRead: (String) -> Unit) {
        if (!haveCrashStored()) {
            return
        }
        val content: String = getCrashFileOrNull()?.readText() ?: return
        onCrashFileRead(content)
    }

    fun removeCrashIfPresent() {
        if (!haveCrashStored()) {
            return
        }
        getCrashFileOrNull()?.delete()
    }

    private fun getCrashFileOrNull(): File? {
        val dir: File = storageLocation ?: return null
        return File(dir, "crash.crash")
    }
}