#pragma once

#include <string>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

namespace console {

class SyslogReporter {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the SyslogReporter instance
     */
    static SyslogReporter& getInstance()
    {
        static SyslogReporter instance;
        return instance;
    }

    /**
     * @brief Initialize syslog reporter with server parameters
     * @param server Syslog server address
     * @param port Server port
     * @param hostname Local hostname for identification
     * @return Reference to this SyslogReporter instance
     */
    SyslogReporter& init(const std::string& server, int port, const std::string& hostname);

    /**
     * @brief Send syslog message with priority
     * @param priority Syslog priority level
     * @param tag String to prepend before the formatted message
     * @param fmt printf-style format string
     * @param ... Variable arguments
     * @return true if sent successfully, false if not initialized or send failed
     */
    bool send(int priority, const char *tag, const char *fmt, ...);

    SyslogReporter(const SyslogReporter&) = delete;
    SyslogReporter& operator=(const SyslogReporter&) = delete;

private:
    SyslogReporter();
    ~SyslogReporter();

    bool mInitialized;
    std::string mHostname;
    std::string mServer;
    int mPort;
};

} // namespace
