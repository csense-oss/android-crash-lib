package com.csense.android.crashnative

import android.app.Application
import android.util.*
import com.csense.android.crashhandler.*

class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        CrashHandler.install(
            log = true,
            storageLocation = this.cacheDir
        )

    }

}