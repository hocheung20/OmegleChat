#ifndef OmegleAPI_H
#define OmegleAPI_H

#include <iostream>
#include <map>

#include "json/json.h"

#include "boost/network/protocol/http/client.hpp"
#include "boost/network/uri.hpp"

class OmegleAPI
{
public:
    static boost::network::http::client client_;
    
public:
    enum OmegleEventType
    {
        OMEGLE_EVENT_CONNECTED,
        OMEGLE_EVENT_WAITING,
        OMEGLE_EVENT_STATUS_INFO,
        OMEGLE_EVENT_IDENT_DIGESTS,
        OMEGLE_EVENT_GOT_MESSAGE,
        OMEGLE_EVENT_TYPING,
        OMEGLE_EVENT_STOPPED_TYPING,
        OMEGLE_EVENT_STRANGER_DISCONNECTED,
        OMEGLE_EVENT_RECAPTCHA_REQUIRED,
        OMEGLE_EVENT_UNRECOGNIZED //Uh oh
    };
    
    class OmegleStartInfo
    {
    private:
        std::string clientID;
        std::string serverName;
    public:
        OmegleStartInfo(std::string clientID, std::string serverName) :
            clientID(clientID),
            serverName(serverName)
        {}
        
        std::string getClientID() { return clientID; }
        std::string getServerName() { return serverName; }
    };
    
    typedef std::pair< OmegleEventType, std::string > OmegleEvent;
    
    static OmegleStartInfo start(); //returns unique session id
    
    static void events(std::string clientId, std::string serverName);
    
    static OmegleEvent parseEvent(Json::Value event);
    
    static void send(std::string clientID, std::string serverName, std::string message);
};

#endif //OmegleAPI_H