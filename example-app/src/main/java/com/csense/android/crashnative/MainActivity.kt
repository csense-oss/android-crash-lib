package com.csense.android.crashnative

import android.os.*
import android.util.*
import androidx.appcompat.app.*
import com.csense.android.crashhandler.*
import com.csense.android.crashnative.databinding.*

class MainActivity : AppCompatActivity() {


    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.crashNative.setOnClickListener {
            CrashHandler.crashNative()
        }
        binding.crashJava.setOnClickListener {
            throw RuntimeException("java crash")
        }
        binding.clearCrash.setOnClickListener {
            CrashHandler.removeCrashIfPresent()
            binding.lastCrashMessage.setText("")
        }
        readLastCrash()
    }

    fun readLastCrash() {
        val haveCrash: Boolean = CrashHandler.haveCrashStored()
        Log.e("crash", "have crash?: $haveCrash")
        if (haveCrash) {
            readCrash()
        }
    }


    private fun readCrash() {
        CrashHandler.tryReadCrash { it: String ->
            binding.lastCrashMessage.setText(it)
        }
    }

}