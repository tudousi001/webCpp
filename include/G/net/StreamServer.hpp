#ifndef _STREAMSERVER_HPP_
#define _STREAMSERVER_HPP_

extern "C"
{
    #include "G/net/Gnet.h"
}

#include "G/net/Server.hpp"
#include "G/net/StreamSocket.hpp"

namespace G {
    typedef StreamSocket* (*handle_connect_fn)(SOCKET);

    class StreamServer: virtual public G::Object, virtual public G::Server
    {
        handle_connect_fn handler;
    public:
        StreamServer(handle_connect_fn);
        virtual int service(int) override;
    };

}

#endif
