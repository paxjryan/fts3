/*
 *	Copyright notice:
 *	Copyright © Members of the EMI Collaboration, 2010.
 *
 *	See www.eu-emi.eu for details on the copyright holders
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 * GetLogCli.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: Michal Simon
 */

#include "GetLogCli.h"

using namespace fts3::cli;

GetLogCli::GetLogCli()
{

    // add commandline options specific for fts3-transfer-submit
    specific.add_options()
    ("path,p", "Destination path for the log files.")
    ;
}

GetLogCli::~GetLogCli()
{
}

string GetLogCli::getPath()
{

    // check whether jobid has been given as a parameter
    if (vm.count("path"))
        {
            return vm["path"].as<string>();
        }

    return string();
}

