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
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or impltnsied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 * OsgParser.h
 *
 *  Created on: Jan 14, 2013
 *      Author: Michał Simon
 */

#ifndef OSGPARSER_H_
#define OSGPARSER_H_

#include "common/ThreadSafeInstanceHolder.h"

#include <pugixml.hpp>
#include <string>
#include <boost/optional.hpp>

namespace fts3
{
namespace infosys
{

using namespace std;
using namespace boost;
using namespace pugi;

using namespace fts3::common;

/**
 * OsgParser class is for parsing MyOSG XML files
 *
 * It has a singleton access.
 *
 * @see ThreadSafeInstanceHolder
 */
class OsgParser : public ThreadSafeInstanceHolder<OsgParser>
{

    friend class ThreadSafeInstanceHolder<OsgParser>;

public:

    /**
     * Destructor
     */
    virtual ~OsgParser();

    /**
     * Gets the site name for the given SE name
     *
     * @param fqdn - fully qualified name of the SE
     *
     * @return the site name
     */
    string getSiteName(string fqdn);

    /**
     * Checks is the given SE is active
     *
     * @param fqdn - fully qualified name of the SE
     *
     * @return true if the SE is active, false otherwise
     */
    optional<bool> isActive(string fqdn);

    /**
     * Checks is the given SE is disabled
     *
     * @param fqdn - fully qualified name of the SE
     *
     * @return true if the SE is disabled, false otherwise
     */
    optional<bool> isDisabled(string fqdn);

private:

    /**
     * Constructor
     */
    OsgParser(string path = myosg_path);

    /// not implemented
    OsgParser(OsgParser const&);

    /// not implemented
    OsgParser& operator=(OsgParser const&);

    /**
     * Gets a property for the given SE name
     *
     * @param fqdn - fully qualified name of the SE
     * @param property - the property of interest
     *
     * @return the value of the property
     */
    string get(string fqdn, string property);

    /**
     * Checks in fts3config if MyOSG is in use
     *
     * @return true if MyOSG is in use, false otherwise
     */
    bool isInUse();

    /// name property node
    static const string NAME_PROPERTY;
    /// active property node
    static const string ACTIVE_PROPERTY;
    /// disabled property node
    static const string DISABLE_PROPERTY;

    /// 'true' string
    static const string STR_TRUE;

    /// the xml document that is being parsed
    xml_document doc;

    /// xpath for the given SE name
    static string xpath_fqdn(string fqdn);
    /// xpath in case the SE name is an alias
    static string xpath_fqdn_alias(string alias);

    /// default path to MyOSG file
    static const string myosg_path;
};

} /* namespace cli */
} /* namespace fts3 */
#endif /* OSGPARSER_H_ */
