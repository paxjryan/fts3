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
 */

#include "gsoap_proxy.h"
#include "SrvManager.h"
#include "ui/TransferStatusCli.h"

#include "common/JobStatusHandler.h"

#include <vector>
#include <string>

using namespace std;
using namespace fts3::cli;
using namespace fts3::common;

/**
 * This is the entry point for the fts3-transfer-status command line tool.
 */
int main(int ac, char* av[]) {

	soap* soap = soap_new();
	// get SrvManager instance
	SrvManager* manager = SrvManager::getInstance();

	try {
		// create and the command line utility
		TransferStatusCli cli;
		cli.initCli(ac, av);

		// if applicable print help or version and exit
		if (cli.printHelp(av[0]) || cli.printVersion()) return 0;

		// get the FTS3 service endpoint
		string endpoint = cli.getService();

		// check if its not empty
		if (endpoint.empty()) {
			cout << "The service has not been defined." << endl;
			return 0;
		}

		// get job IDs that have to be check
		vector<string> jobIds = cli.getJobIds();
		if (jobIds.empty()) {
			cout << "No request ID specified." << endl;
			return 0;
		}

		//service.soap_endpoint = "https://vtb-generic-32.cern.ch:8443/glite-data-transfer-fts/services/FileTransfer";
		// set the endpoint

		// initialize SOAP
		if (!manager->initSoap(soap, endpoint)) return 0;

		// initialize SrvManager
		if (!manager->init(soap, endpoint.c_str())) return 0;

		// if verbose print general info
		if (cli.isVerbose()) {
			cli.printGeneralInfo();
		}

		// iterate over job IDs
		vector<string>::iterator it;
		for (it = jobIds.begin(); it < jobIds.end(); it++) {

			//string job = "1215375b-5407-11e1-9322-bc3d2ed9263b"
			string jobId = *it;
			int ret;

			if (cli.isVerbose()) {

				if (manager->isItVersion330()) {
					// if version higher than 3.3.0 use getTransferJobSummary2

					// do the request
					impltns__getTransferJobSummary2Response resp;
					ret = soap_call_impltns__getTransferJobSummary2(soap, endpoint.c_str(), 0, jobId, resp);

					// print the response
					if (!ret && resp._getTransferJobSummary2Return) {

						JobStatusHandler::printJobStatus(resp._getTransferJobSummary2Return->jobStatus);

					    cout << "\tDone: " << resp._getTransferJobSummary2Return->numDone << endl;
					    cout << "\tActive: " << resp._getTransferJobSummary2Return->numActive << endl;
					    cout << "\tPending: " << resp._getTransferJobSummary2Return->numPending << endl;
					    cout << "\tReady: " << resp._getTransferJobSummary2Return->numReady << endl;
					    cout << "\tCanceled: " << resp._getTransferJobSummary2Return->numCanceled << endl;
					    cout << "\tFailed: " << resp._getTransferJobSummary2Return->numFailed << endl;
					    cout << "\tFinishing: " << resp._getTransferJobSummary2Return->numFinishing << endl;
					    cout << "\tFinished: " << resp._getTransferJobSummary2Return->numFinished << endl;
					    cout << "\tSubmitted: " << resp._getTransferJobSummary2Return->numSubmitted << endl;
					    cout << "\tHold: " << resp._getTransferJobSummary2Return->numHold << endl;
					    cout << "\tWaiting: " << resp._getTransferJobSummary2Return->numWaiting << endl;
					}

				} else {
					// if version higher than 3.3.0 use getTransferJobSummary

					// do the request
					impltns__getTransferJobSummaryResponse resp;
					ret = soap_call_impltns__getTransferJobSummary(soap, endpoint.c_str(), 0, jobId, resp);

					// print the response
					if (!ret && resp._getTransferJobSummaryReturn) {

						JobStatusHandler::printJobStatus(resp._getTransferJobSummaryReturn->jobStatus);

					    cout << "\tDone: " << resp._getTransferJobSummaryReturn->numDone << endl;
					    cout << "\tActive: " << resp._getTransferJobSummaryReturn->numActive << endl;
					    cout << "\tPending: " << resp._getTransferJobSummaryReturn->numPending << endl;
					    cout << "\tCanceled: " << resp._getTransferJobSummaryReturn->numCanceled << endl;
					    cout << "\tCanceling: " << resp._getTransferJobSummaryReturn->numCanceling << endl;
					    cout << "\tFailed: " << resp._getTransferJobSummaryReturn->numFailed << endl;
					    cout << "\tFinished: " << resp._getTransferJobSummaryReturn->numFinished << endl;
					    cout << "\tSubmitted: " << resp._getTransferJobSummaryReturn->numSubmitted << endl;
					    cout << "\tHold: " << resp._getTransferJobSummaryReturn->numHold << endl;
					    cout << "\tWaiting: " << resp._getTransferJobSummaryReturn->numWaiting << endl;
					    cout << "\tCatalogFailed: " << resp._getTransferJobSummaryReturn->numCatalogFailed << endl;
					}
				}
			} else {

				// do the request
				impltns__getTransferJobStatusResponse resp;
				ret = soap_call_impltns__getTransferJobStatus(soap, endpoint.c_str(), 0, jobId, resp);

		    	// print the response
		    	if (!ret && resp._getTransferJobStatusReturn) {
		    		cout << *resp._getTransferJobStatusReturn->jobStatus << endl;
		    	}
			}

			// TODO test!
			// check if the -l option has been used
			if (cli.list()) {

				// do the request
				impltns__getFileStatusResponse resp;
				ret = soap_call_impltns__getFileStatus(soap, endpoint.c_str(), 0, jobId, 0, 100, resp);

				if (!ret && resp._getFileStatusReturn) {

					std::vector<tns3__FileTransferStatus * >& vect = resp._getFileStatusReturn->item;
					std::vector<tns3__FileTransferStatus * >::iterator it;

					// print the response
					for (it = vect.begin(); it < vect.end(); it++) {
						tns3__FileTransferStatus* stat = *it;

						cout << "  Source:      " << *stat->sourceSURL << endl;
						cout << "  Destination: " << *stat->destSURL << endl;
						cout << "  State:       " << *stat->transferFileState << endl;;
						cout << "  Retries:     " << stat->numFailures << endl;
						cout << "  Reason:      " << *stat->reason << endl;
						cout << "  Duration:    " << stat->duration << endl;
					}
				}
			}

			// print the error message if applicable
	    	if (ret) {
	    		cout << "getTransferJobStatus: ";
	    		manager->printSoapErr(soap);
	    	}
		}
    }
    catch(std::exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
        
    } catch(...) {
        cerr << "Exception of unknown type!\n";
    }

	soap_destroy(soap);
	soap_end(soap);
	soap_free(soap);

	return 0;
}
