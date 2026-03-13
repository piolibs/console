/*
 * Copyright (C) 2026 Dmitry Korobkov <dmitry.korobkov.nn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <cstdio>
#include "SyslogReporter.h"

namespace console {

SyslogReporter::SyslogReporter()
  : mInitialized(false),
    mHostname(),
    mServer(),
    mPort(0)
{}

SyslogReporter::~SyslogReporter()
{}

SyslogReporter& SyslogReporter::init(const std::string &server, int port, const std::string &hostname)
{
    mServer = server;
    mHostname = hostname;
    mPort = port;

    mInitialized = true;
    return *this;
}

bool SyslogReporter::send(int priority, const char *tag, const char *fmt, ...)
{
    WiFiUDP udp;
    char mBuffer[96];

    if (!mInitialized || (WiFi.status() != WL_CONNECTED))
    {
        return false;
    }

    if (udp.beginPacket(mServer.c_str(), mPort))
    {
        int size = snprintf(mBuffer, sizeof(mBuffer), "<%u> %s %s ",
                            priority, mHostname.c_str(), (tag) ? tag : "--");
        udp.write((const uint8_t*)mBuffer, size);

        va_list args;
        va_start(args, fmt);
        size = vsnprintf(mBuffer, sizeof(mBuffer), fmt, args);
        va_end(args);

        udp.write((const uint8_t *)mBuffer, size);

        udp.endPacket();

        return true;
    }

    return false;
}

} // namespace