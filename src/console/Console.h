/*
 * Copyright (C) 2026 Dmitry Korobkov <dmitry.korobkov.nn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <Arduino.h>

// Log levels

#define LOG_LEVEL_VERBOSE 5
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_NONE 0

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#ifndef LOG_MODULE
#define LOG_MODULE
#endif

// Helper macros

#define _LOG_TAG_1(level) "[" level "]"
#define _LOG_TAG_2(level, module) "[" level "][" module "]"

#define _LOG_TAG_SELECT(_1, _2, NAME, ...) NAME
#define _LOG_TAG_EXPAND(level, ...) \
    _LOG_TAG_SELECT(level __VA_OPT__(, ) __VA_ARGS__, _LOG_TAG_2, _LOG_TAG_1) \
                   (level __VA_OPT__(, ) __VA_ARGS__)
#define _LOG_TAG(level) _LOG_TAG_EXPAND(level, LOG_MODULE)

#define LOGE_TAG _LOG_TAG("E")
#define LOGW_TAG _LOG_TAG("W")
#define LOGI_TAG _LOG_TAG("I")
#define LOGD_TAG _LOG_TAG("D")
#define LOGV_TAG _LOG_TAG("V")

#define _CONSOLE_LOG(...) console::Console::getInstance().log(__VA_ARGS__)
#define _CONSOLE_FORMAT(...) console::Console::getInstance().format(__VA_ARGS__)
#define _CONSOLE_FLUSH(...) console::Console::getInstance().flush(__VA_ARGS__)

// Conditional logging macros

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOGE(...) _CONSOLE_LOG(LOGE_TAG " " __VA_ARGS__)
#define LOGE_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOGE_FLUSH() _CONSOLE_FLUSH(LOGE_TAG " ")
#else
#define LOGE(...) ((void)0)
#define LOGE_ADD(...) ((void)0)
#define LOGE_FLUSH() ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOGW(...) _CONSOLE_LOG(LOGW_TAG " " __VA_ARGS__)
#define LOGW_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOGW_FLUSH() _CONSOLE_FLUSH(LOGW_TAG " ")
#else
#define LOGW(...) ((void)0)
#define LOGW_ADD(...) ((void)0)
#define LOGW_FLUSH() ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOGI(...) _CONSOLE_LOG(LOGI_TAG " " __VA_ARGS__)
#define LOGI_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOGI_FLUSH() _CONSOLE_FLUSH(LOGI_TAG " ")
#else
#define LOGI(...) ((void)0)
#define LOGI_ADD(...) ((void)0)
#define LOGI_FLUSH() ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOGD(...) _CONSOLE_LOG(LOGD_TAG " " __VA_ARGS__)
#define LOGD_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOGD_FLUSH() _CONSOLE_FLUSH(LOGD_TAG " ")
#else
#define LOGD(...) ((void)0)
#define LOGD_ADD(...) ((void)0)
#define LOGD_FLUSH() ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define LOGV(...) _CONSOLE_LOG(LOGV_TAG " " __VA_ARGS__)
#define LOGV_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOGV_FLUSH() _CONSOLE_FLUSH(LOGV_TAG " ")
#else
#define LOGV(...) ((void)0)
#define LOGV_ADD(...) ((void)0)
#define LOGV_FLUSH() ((void)0)
#endif

// Backward compatibility

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG(...) _CONSOLE_LOG(LOGI_TAG " " __VA_ARGS__)
#define LOG_ADD(...) _CONSOLE_FORMAT(__VA_ARGS__)
#define LOG_FLUSH() _CONSOLE_FLUSH(LOGI_TAG " ")
#else
#define LOG(...) ((void)0)
#define LOG_ADD(...) ((void)0)
#define LOG_FLUSH() ((void)0)
#endif

// Test environment

#define TEST_LOG(...) _CONSOLE_LOG(__VA_ARGS__)

namespace console
{
    /**
     * @brief Singleton logger class for console output
     *
     * Provides formatted logging with different log levels and
     * incremental message building capabilities.
     */
    class Console
    {
    private:
        Console();
        ~Console();

        Console(Console const &) = delete;
        Console &operator=(Console const &) = delete;

    public:
        /**
         * @brief Get the singleton instance
         * @return Reference to the Console instance
         */
        static Console &getInstance()
        {
            static Console console;
            return console;
        }

        /**
         * @brief Set the serial port for output
         * @param serial Reference to HardwareSerial object
         * @return Reference to this Console instance (for chaining)
         */
        Console &setSerial(HardwareSerial &serial);

        /**
         * @brief Set the baud rate for serial communication
         * @param baudrate Baud rate (e.g., 115200, 9600)
         * @return Reference to this Console instance (for chaining)
         */
        Console &setBaudRate(unsigned long baudrate);

        /**
         * @brief Append formatted text to the internal buffer
         * @param fmt printf-style format string
         * @param ... Variable arguments
         */
        void format(const char *fmt, ...);

        /**
         * @brief Flush the internal buffer to serial output
         * @param prefix Optional string to prepend before the buffered content
         *               (default: empty string)
         */
        void flush(const char *prefix = "");

        /**
         * @brief Print formatted message directly to serial
         * @param fmt printf-style format string
         * @param ... Variable arguments
         *
         * This is equivalent to format() + flush() in one call.
         */
        void log(const char *fmt, ...);

        /**
         * @brief Append formatted text to the internal buffer
         * @param fmt printf-style format string
         * @param args va_list of arguments
         */
        void vformat(const char *fmt, va_list args);

        /**
         * @brief Print message to serial directly to serial
         * @param fmt printf-style format string
         * @param args va_list of arguments
         */
        void vlog(const char *fmt, va_list args);

    private:
        HardwareSerial *serial; // Pointer to serial port
        unsigned long baudrate; // Current baud rate
        char mBuffer[96];       // Internal buffer for message building
    };

    // Console wrapper functions

    void log(const char *fmt, ...);
    void format(const char *fmt, ...);
    void flush(const char *prefix = "");

} // namespace
