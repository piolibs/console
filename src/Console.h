/*
 * Console Logger Library
 *
 * Copyright (C) 2026 Dmitry Korobkov <dmitry.korobkov.nn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>

// Log levels
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_NONE 0

// Base log macro
#define _LOG(...) app::console.log(__VA_ARGS__)

// Conditional logging macros
#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_E(...) _LOG(__VA_ARGS__)
#else
#define LOG_E(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_I(...) _LOG(__VA_ARGS__)
#else
#define LOG_I(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define LOG_W(...) _LOG(__VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_D(...) _LOG(__VA_ARGS__)
#else
#define LOG_D(...)
#endif

// For backward compatibility
#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG(...) _LOG(__VA_ARGS__)
#else
#define LOG(...)
#endif

namespace app
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
         *
         * Prints the accumulated buffer content with newline
         * and clears the buffer.
         */
        void flush();

        /**
         * @brief Print formatted message directly to serial
         * @param fmt printf-style format string
         * @param ... Variable arguments
         *
         * This is equivalent to format() + flush() in one call.
         */
        void log(const char *fmt, ...);

    private:
        HardwareSerial *serial; // Pointer to serial port
        unsigned long baudrate; // Current baud rate
        char mBuffer[96];       // Internal buffer for message building
    };

    // Global instance reference
    extern Console &console;

} // namespace app