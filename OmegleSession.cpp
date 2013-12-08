#include "OmegleSession.h"

OmegleSession::OmegleSession()
{
    clientID = "";
    
    connected_ = false;
    connecting_ = false;

}

void OmegleSession::connect()
{
    OmegleAPI::OmegleStartInfo startInfo = OmegleAPI::start();
}

