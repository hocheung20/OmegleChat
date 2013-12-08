#include <iostream>

#include "boost/network/protocol/http/client.hpp"
#include "boost/network/uri.hpp"
#include "boost/network/uri/uri_io.hpp"

#include "json/json.h"

#include "OmegleAPI.h"
#include "OmegleSession.h"

int main(int argc, char *argv[])
{
    unsigned int session_num = 1;
    
    OmegleSession session1, session2;
    
    session1.connect();
    session2.connect();
}