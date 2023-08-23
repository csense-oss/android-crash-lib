package com.csense.android.crashhandler

class NativeCrashException(
    message: String,
    val signalNumber: Int,
    val signalErrorNumber: Int,
    val signalCode: Int
) : Exception(
    message
) {
    override fun toString(): String {
        return """NativeCrashException(
            |   message=$message,
            |   signalNumber=$signalNumber, 
            |   signalErrorNumber=$signalErrorNumber, 
            |   signalCode=$signalCode
            |)"""
            .trimMargin()
    }
}