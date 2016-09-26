/* Robot 7 Config
 *
 */

#include <EEPROM.h>
#include "r7config.h"

R7Config::R7Config() : _networkId(DEFAULT_NETWORKID), _nodeId(DEFAULT_NODEID)
{
    load();
}

void R7Config::load()
{
    _networkId = DEFAULT_NETWORKID;
    _nodeId = DEFAULT_NODEID;

    // Value at address 0: 0 = use stored settings, > 0 = use defaults
    if (EEPROM.read(USE_DEFAULTS_ADDR) == false) {
        _networkId = EEPROM.read(NETWORKID_ADDRESS);
        _nodeId = EEPROM.read(NODEID_ADDRESS);
    }
}

void R7Config::store()
{
    // Start using EEPROM settings
    EEPROM.write(USE_DEFAULTS_ADDR, false);
    EEPROM.write(NETWORKID_ADDRESS, _networkId);
    EEPROM.write(NODEID_ADDRESS, _nodeId);
}

void R7Config::prettyPrint( String& ouput )
{
    ouput += "Network ID: ";
    ouput += _networkId;
    ouput += ", Node ID:";
    ouput += _nodeId;
}

void R7Config::setId(const byte& networkId, const byte& nodeId) {
    _networkId = networkId;
    _nodeId = nodeId;
    store();
}

