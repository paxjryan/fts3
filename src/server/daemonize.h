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

/** \file daemonize.h Implement FTS3 server daemon code. */

#pragma once

#include "server_dev.h"

FTS3_SERVER_NAMESPACE_START

/** Puts the server into daemon mode. */
void daemonize();

FTS3_SERVER_NAMESPACE_END

