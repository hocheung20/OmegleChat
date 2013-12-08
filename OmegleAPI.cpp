#include "boost/network/protocol/http/client.hpp"
#include "boost/network/uri/uri_io.hpp"
#include "json/json.h"

#include "OmegleAPI.h"

boost::network::http::client OmegleAPI::client_ = boost::network::http::client();

using namespace boost::network;

//Returns unique session string
OmegleAPI::OmegleStartInfo OmegleAPI::start()
{
    using namespace boost::network;
    
    uri::uri server_url;
    server_url << uri::scheme("http") << uri::host("omegle.com");
    
    //Post /start
    uri::uri start_url;
    start_url << server_url << uri::path("/start");
    start_url << uri::query("rcs", "1") <<
    uri::query("firstevents", "1") <<
    uri::query("spid", "") <<
    uri::query("randid", "JKTTE5A6") <<
    uri::query("lang", "en");
    
    http::client::request request_start(start_url);
    request_start << header("User-Agent", "runscope/0.1");
    request_start << header("Connection", "close");
    
    std::cout << "Posted: " << request_start.uri().string() << std::endl;
    
    http::client::response start_response = OmegleAPI::client_.post(request_start);
    
    std::string start_response_body = body(start_response);
    
    //Parse the start response
    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(start_response_body, root))
    {
        std::cerr << __PRETTY_FUNCTION__ << ": Error parsing json response!" << std::endl;
        std::terminate();
    }
    
    std::cout << root << std::endl;
    
    std::string clientID = root["clientID"].asString();
    
    Json::Value events = root["events"]; //events should always be an array
    
    
    std::string serverName;
    //events should always contain statusInfo
    for (int k = 0; k < events.size(); ++k)
    {
        Json::Value event = events[k]; //event should always be an array
        
        OmegleEvent omegleEvent = parseEvent(event);
        
        if (omegleEvent.first == OMEGLE_EVENT_STATUS_INFO)
        {
            serverName = omegleEvent.first;
        }
    }
        
    return OmegleStartInfo(clientID, serverName);
}

void OmegleAPI::events(std::string clientID, std::string serverName)
{
    uri::uri events_url;
    events_url << uri::scheme("http") << uri::host(serverName) << uri::path("/events");
    http::client::request request_events(events_url);
    request_events << header("User-Agent", "runscope/0.1");
    request_events << header("Connection", "close");
    
    std::string data = "id=" + uri::encoded(clientID);
    
    http::client::response events_response = OmegleAPI::client_.post(request_events, data, std::string("application/x-www-form-urlencoded"));
    //std::cout << "Posting: " << request_events.uri().string() << std::endl;
    
    std::string events_response_body = body(events_response);
    
    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(events_response_body, root))
    {
        std::cerr << __PRETTY_FUNCTION__ << ": Error parsing json response!" << std::endl;
        std::terminate();
    }
    
    //std::cout << root << std::endl;
    
    for (int k = 0; k < root.size(); ++k)
    {
        parseEvent(root[k]);
    }
}

void OmegleAPI::send(std::string clientID, std::string serverName, std::string message)
{
    uri::uri events_url;
    events_url << uri::scheme("http") << uri::host(serverName) << uri::path("/send");
    http::client::request request_events(events_url);
    request_events << header("User-Agent", "runscope/0.1");
    request_events << header("Connection", "close");
    
    std::string data = "id=" + uri::encoded(clientID) + "&msg=" + uri::encoded(message);
    
    http::client::response events_response = OmegleAPI::client_.post(request_events, data, std::string("application/x-www-form-urlencoded"));
}

OmegleAPI::OmegleEvent OmegleAPI::parseEvent(Json::Value event)
{
    std::string eventType = (*event.begin()).asString();
    //std::cout << eventType << std::endl;
    
    if (!eventType.compare("connected"))
    {
        std::cout << "Got connected" << std::endl;
        return OmegleEvent(OMEGLE_EVENT_CONNECTED, "");
    }
    else if (!eventType.compare("waiting"))
    {
        std::cout << "Got waiting" << std::endl;
        
        return OmegleEvent(OMEGLE_EVENT_WAITING, "");
    }
    else if (!eventType.compare("statusInfo"))
    {
        std::cout << "Got status info" << std::endl;
        Json::Value servers = (*(++event.begin()))["servers"];
        boost::network::uri::uri server_url_;
        server_url_ << uri::scheme("http") << uri::host((*servers.begin()).asString());
        
        return OmegleEvent(OMEGLE_EVENT_STATUS_INFO, server_url_.string());
    }
    else if (!eventType.compare("identDigests"))
    {
        std::cout << "Got identDigests" << std::endl;
        return OmegleEvent(OMEGLE_EVENT_IDENT_DIGESTS, ""); //TODO: pass back digests? (no use right now)
    }
    else if (!eventType.compare("gotMessage"))
    {
        std::cout << "Got message: " << event[1] << std::endl;
        return OmegleEvent(OMEGLE_EVENT_GOT_MESSAGE, event[1].asString());
    }
    else if (!eventType.compare("typing"))
    {
        std::cout << "Got typing" << std::endl;
        return OmegleEvent(OMEGLE_EVENT_TYPING, "");
    }
    else if (!eventType.compare("stoppedTyping"))
    {
        std::cout << "Got stopped typing" << std::endl;
        return OmegleEvent(OMEGLE_EVENT_STOPPED_TYPING, "");
    }
    else if (!eventType.compare("strangerDisconnected"))
    {
        std::cout << "Got stranger disconnected" << std::endl;
        
        return OmegleEvent(OMEGLE_EVENT_STRANGER_DISCONNECTED, "");
    }
    else if (!eventType.compare("recaptchaRequired"))
    {
        std::cout << "Got Recaptcha required" << std::endl;
        
        return OmegleEvent(OMEGLE_EVENT_RECAPTCHA_REQUIRED, "");
    }
    else
    {
        std::cerr << "Unrecognized event: " << event << std::endl;
        return OmegleEvent(OMEGLE_EVENT_UNRECOGNIZED, event.asString());
    }
    
    assert(!"Somehow we are not handling an event");
    return OmegleEvent(OMEGLE_EVENT_UNRECOGNIZED, event.asString());
}