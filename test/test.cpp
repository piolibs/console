/*
 * Copyright (C) 2026 Dmitry Korobkov <dmitry.korobkov.nn@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <Arduino.h>
#include <unity.h>

#include <console.h>

using namespace console;

void setUp()
{
    TEST_LOG("setup fixture");
}

void test_basic(void)
{
    TEST_LOG("--- TEST CASE: %s ---", __func__);

    LOG("info");
    LOGE("error");
    LOGW("warning");
    LOGI("info");
    LOGD("debug");
    LOGV("verbose");

    {
        char expected[120];
        char *npos = expected;
        for (unsigned int ix = 0; ix < sizeof(expected) / 10; ++ix)
        {
            npos += sprintf(npos, "%02u:456789:", ix + 1);
        }

        LOG(expected);
        LOGE(expected);
        LOGW(expected);
        LOGI(expected);
        LOGD(expected);
        LOGV(expected);
    }
}

void test_add_flush(void)
{
    TEST_LOG("--- TEST CASE: %s ---", __func__);

    {
        char expected[120];
        char* npos = expected;
        for (unsigned int ix = 0; ix < sizeof(expected) / 10; ++ix)
        {
            npos += sprintf(npos, "%02u:456789:", ix + 1);
        }

        expected[50] = '\0';

        LOGI_ADD(expected);
        LOGI_FLUSH();

        LOGI_ADD(expected);
        LOGI_ADD(expected);
        LOGI_ADD(expected);
        LOGI_FLUSH();
    }

    {
        char expected[120];
        char *npos = expected;
        for (unsigned int ix = 0; ix < sizeof(expected) / 10; ++ix)
        {
            npos += sprintf(npos, "%02u:456789:", ix + 1);
        }

        expected[100] = '\0';

        LOGI_ADD(expected);
        LOGI_FLUSH();
    }
}

void setup()
{
    Serial.begin(74880);
    delay(1000);

    Serial.println("\n\n");
    Serial.flush();

    UNITY_BEGIN();
    RUN_TEST(test_basic);
    RUN_TEST(test_add_flush);
    UNITY_END();
}

void loop()
{
    delay(1000);
}