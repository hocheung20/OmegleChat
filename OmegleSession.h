#ifndef OmegleSession_H
#define OmegleSession_H

#include "boost/network/protocol/http/client.hpp"
#include "boost/network/uri.hpp"

#include "OmegleAPI.h"

class OmegleSession
{    
private:
    bool connected_;
    bool connecting_;
    
    boost::network::uri::uri server_url_;
    
    std::string clientID;
    
public:
    OmegleSession();
    
    bool isConnected() { return connected_; }
    bool isConnecting() { return connecting_; }
    
    void connect();
};

#endif