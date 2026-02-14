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

#include <cstdio>
#include <cstdarg>
#include <cstring>

#include "Console.h"

#ifndef SERIAL_BAUD_RATE
#define SERIAL_BAUD_RATE 115200
#endif

namespace app
{

    // Global instance reference
    Console &console = Console::getInstance();

    Console::Console() : serial(&Serial),
                         baudrate(SERIAL_BAUD_RATE),
                         mBuffer{0}
    {
        if (serial)
        {
            serial->begin(this->baudrate);
        }
    }

    Console::~Console() {}

    Console &Console::setSerial(HardwareSerial &serial)
    {
        this->serial = &serial;
        if (this->serial)
        {
            this->serial->begin(this->baudrate);
        }
        return *this;
    }

    Console &Console::setBaudRate(unsigned long baudrate)
    {
        this->baudrate = baudrate;
        if (serial)
        {
            serial->begin(this->baudrate);
        }
        return *this;
    }

    void Console::format(const char *fmt, ...)
    {
        if (!serial)
            return;

        size_t currentLen = strlen(mBuffer);
        char *npos = mBuffer + currentLen;
        size_t remaining = sizeof(mBuffer) - currentLen - 1;

        va_list ptr;
        va_start(ptr, fmt);
        vsnprintf(npos, remaining, fmt, ptr);
        va_end(ptr);
    }

    void Console::flush()
    {
        if (!serial)
            return;

        if (strlen(mBuffer) > 0)
        {
            serial->println(mBuffer);
            mBuffer[0] = '\0';
        }
    }

    void Console::log(const char *fmt, ...)
    {
        if (!serial)
            return;

        va_list ptr;
        va_start(ptr, fmt);
        vsnprintf(mBuffer, sizeof(mBuffer) - 1, fmt, ptr);
        va_end(ptr);

        this->flush();
    }

} // namespace app