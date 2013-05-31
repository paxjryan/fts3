/* Copyright @ Members of the EMI Collaboration, 2010.
See www.eu-emi.eu for details on the copyright holders.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

/** \file logger.cpp Test of Logger macros. */

#include "logger.h"

#ifdef FTS3_COMPILE_WITH_UNITTEST
#include "unittest/testsuite.h"
#endif // FTS3_COMPILE_WITH_UNITTESTS

/* ---------------------------------------------------------------------- */

FTS3_COMMON_NAMESPACE_START

#ifdef FTS3_COMPILE_WITH_UNITTEST

BOOST_FIXTURE_TEST_CASE (Common__Logger_Newlog_Macros, Logger)
{
    std::stringstream f_str;
    f_str << "thread:" << ThreadTraits::get_id() << _separator();
    // Non-debug level logging
    FTS3_COMMON_LOGGER_NEWLOG(INFO);
    BOOST_CHECK_EQUAL (f_str.str(), theLogger()._logLine.str());
    // Non-debug level logging
    // They MUST be in one source code line!
    FTS3_COMMON_LOGGER_NEWLOG(DEBUG);
    const int f_lineno = __LINE__;
    const char* f_file = __FILE__;
    const char* f_function = __FUNCTION__;
    f_str.str("");

    f_str << "thread:" << ThreadTraits::get_id() << _separator() << f_file << _separator()
          << f_function << _separator() << std::dec << f_lineno << _separator();

    BOOST_CHECK_EQUAL (f_str.str(), theLogger()._logLine.str());
}

// Check FTS3_COMMON_LOGGER_LOG macro
BOOST_FIXTURE_TEST_CASE (Common__Logger_Log_Macros, Logger)
{
    BOOST_TEST_MESSAGE ("\n**** This is a visual test. Ensure that the messages below"
                        "are visible (or not visible) in /var/log/syslog. TODO: automate it.\n");

    std::string f_message = "message";
    FTS3_COMMON_LOGGER_LOG(INFO, f_message);
    BOOST_TEST_MESSAGE ("\n**** End of manual test.");
}

#endif // FTS3_COMPILE_WITH_UNITTESTS

FTS3_COMMON_NAMESPACE_END

