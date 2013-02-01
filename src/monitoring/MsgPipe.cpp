/**
 *  Copyright (c) Members of the EGEE Collaboration. 2004.
 *  See http://www.eu-egee.org/partners/ for details on the copyright
 *  holders.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  
 * 
 */


#include "MsgPipe.h"
#include "half_duplex.h" /* For name of the named-pipe */
#include "utility_routines.h"
#include <iostream>
#include <string>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "concurrent_queue.h"
#include "Logger.h"
#include <vector>


extern bool stopThreads;

void handler(int sig) {
    sig = 0;
    stopThreads = true;
    sleep(5);
    exit(0);
}

MsgPipe::MsgPipe(std::string) { 
    //register sig handler to cleanup resources upon exiting
    signal(SIGFPE, handler);
    signal(SIGILL, handler);
    signal(SIGSEGV, handler);
    signal(SIGBUS, handler);
    signal(SIGABRT, handler);
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    
    fd = inotify_init();
    wd = inotify_add_watch( fd, MONITORING_DIR, IN_MODIFY | IN_CREATE | IN_DELETE );    
}

MsgPipe::~MsgPipe() {
    cleanup();
    ( void ) inotify_rm_watch( fd, wd );
    ( void ) close( fd );     
}


void MsgPipe::run() {
   
    std::vector<std::string> messages;
    std::vector<std::string>::const_iterator iter;
    
    while (stopThreads==false){
     try{
	/*blocking call, avoid busy-wating loop*/
        length = read( fd, buffer, BUF_LEN );		     
        runConsumerMonitoring(messages);
	if(!messages.empty()){
		for (iter = messages.begin(); iter != messages.end(); ++iter){			
			concurrent_queue::getInstance()->push(*iter);
		}
	messages.clear();
	}	
        sleep(1);							    
      }   
     catch (...) {
                FTS3_COMMON_EXCEPTION_THROW(Err_Custom("Message queue thrown exception"));
        }
    }
}

void MsgPipe::cleanup() {

}
