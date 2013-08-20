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

#pragma once

#include "server_dev.h"
#include "common/pointers.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <string>
#include "SingleDbInstance.h"
#include "common/logger.h"
#include "common/error.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fstream>
#include "config/serverconfig.h"
#include "definitions.h"
#include "queue_updater.h"
#include <boost/algorithm/string.hpp>
#include "ws/SingleTrStateInstance.h"

extern bool stopThreads;


FTS3_SERVER_NAMESPACE_START
using FTS3_COMMON_NAMESPACE::Pointer;
using namespace FTS3_COMMON_NAMESPACE;
using namespace db;
using namespace FTS3_CONFIG_NAMESPACE;

template
<
typename TRAITS
>
class ProcessUpdaterDBServiceHandler : public TRAITS::ActiveObjectType
{
protected:

    using TRAITS::ActiveObjectType::_enqueue;

public:

    /* ---------------------------------------------------------------------- */

    typedef ProcessUpdaterDBServiceHandler <TRAITS> OwnType;

    /* ---------------------------------------------------------------------- */

    /** Constructor. */
    ProcessUpdaterDBServiceHandler
    (
        const std::string& desc = "" /**< Description of this service handler
            (goes to log) */
    ) :
        TRAITS::ActiveObjectType("ProcessUpdaterDBServiceHandler", desc)
    {
    }

    /* ---------------------------------------------------------------------- */

    /** Destructor */
    virtual ~ProcessUpdaterDBServiceHandler()
    {
    }

    /* ---------------------------------------------------------------------- */

    void executeTransfer_p
    (
    )
    {

        boost::function<void() > op = boost::bind(&ProcessUpdaterDBServiceHandler::executeTransfer_a, this);
        this->_enqueue(op);
    }

protected:
    std::vector<struct message_updater> messages;

    /* ---------------------------------------------------------------------- */
    void executeTransfer_a()
    {
        static unsigned int counter = 0;

        while (stopThreads==false)   /*need to receive more than one messages at a time*/
            {
                try
                    {
                        ThreadSafeList::get_instance().checkExpiredMsg(messages);

                        if (!messages.empty())
                            {
                                bool updated = DBSingleton::instance().getDBObjectInstance()->retryFromDead(messages);
                                if(updated)
                                    {
                                        ThreadSafeList::get_instance().deleteMsg(messages);
                                        std::vector<struct message_updater>::const_iterator iter;
                                        for (iter = messages.begin(); iter != messages.end(); ++iter)
                                            {
                                                SingleTrStateInstance::instance().sendStateMessage((*iter).job_id, (*iter).file_id);
                                            }
                                        messages.clear();
                                    }
                            }

                        /*set to fail all old queued jobs which have exceeded max queue time*/
                        std::vector<std::string> jobs;
                        DBSingleton::instance().getDBObjectInstance()->setToFailOldQueuedJobs(jobs);
                        if(!jobs.empty())
                            {
                                std::vector<std::string>::const_iterator iter2;
                                for (iter2 = jobs.begin(); iter2 != jobs.end(); ++iter2)
                                    {
                                        SingleTrStateInstance::instance().sendStateMessage((*iter2), -1);
                                    }
                                jobs.clear();
                            }
                        messages.clear();

                        counter++;
                        if (counter == 500)
                            {
                                DBSingleton::instance().getDBObjectInstance()->checkSanityState();
                                counter = 0;
                            }	    
                    }
                catch (const std::exception& e)
                    {
                        FTS3_COMMON_LOGGER_NEWLOG(ERR) << "Message updater thrown exception "
                                                       << e.what()
                                                       << commit;
                        sleep(2);
                    }
                catch (...)
                    {
                        FTS3_COMMON_LOGGER_NEWLOG(ERR) << "Message updater thrown unhandled exception" << commit;
                        sleep(2);
                    }
                sleep(2);
            }
    }

    /* ---------------------------------------------------------------------- */
    struct TestHelper
    {

        TestHelper()
            : loopOver(false)
        {
        }

        bool loopOver;
    }
    _testHelper;
};

FTS3_SERVER_NAMESPACE_END

