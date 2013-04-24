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
 * DelCfgCli.h
 *
 *  Created on: Apr 24, 2013
 *      Author: Michal Simon
 */

#ifndef DELCFGCLI_H_
#define DELCFGCLI_H_

#include "SetCfgCli.h"

namespace fts3 {
namespace cli {

/**
 * It's a wraper for SetCfgCli that turns off specific options!
 */
class DelCfgCli : public SetCfgCli {
public:
	DelCfgCli();
	virtual ~DelCfgCli();
};

} /* namespace cli */
} /* namespace fts3 */
#endif /* DELCFGCLI_H_ */
