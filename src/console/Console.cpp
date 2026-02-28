/*
 * Copyright (C) 2026 Dmitry Korobkov <dmitry.korobkov.nn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <cstdio>
#include <cstdarg>
#include <cstring>

#include "Console.h"

#ifndef SERIAL_BAUD_RATE
#define SERIAL_BAUD_RATE 115200
#endif

namespace console
{
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

    void Console::log(const char *fmt, ...)
    {
        if (!serial) return;

        va_list ptr;
        va_start(ptr, fmt);
        vsnprintf(mBuffer, sizeof(mBuffer) - 1, fmt, ptr);
        va_end(ptr);

        this->flush();
    }

    void Console::format(const char *fmt, ...)
    {
        if (!serial) return;

        size_t length = strlen(mBuffer);
        char *npos = mBuffer + length;
        size_t remaining = sizeof(mBuffer) - length - 1;

        va_list args;
        va_start(args, fmt);
        vsnprintf(npos, remaining, fmt, args);
        va_end(args);
    }

    void Console::flush(const char *prefix)
    {
        if (!serial) return;

        if (strlen(mBuffer) > 0)
        {
            serial->print(prefix);
            serial->println(mBuffer);
            serial->flush();
            mBuffer[0] = '\0';
        }
    }

    void Console::vformat(const char *fmt, va_list args)
    {
        if (!serial) return;

        size_t length = strlen(mBuffer);
        char *npos = mBuffer + length;
        size_t remaining = sizeof(mBuffer) - length - 1;

        vsnprintf(npos, remaining, fmt, args);
    }

    void Console::vlog(const char *fmt, va_list args)
    {
        this->vformat(fmt, args);
        this->flush();
    }

    // Console wrapper functions

    void log(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Console::getInstance().vlog(fmt, args);
        va_end(args);
    }

    void format(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        Console::getInstance().vformat(fmt, args);
        va_end(args);
    }

    void flush(const char *prefix)
    {
        Console::getInstance().flush(prefix);
    }

} // namespace
