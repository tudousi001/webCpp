#ifndef _GUNIXSERVER_HPP_
#define _GUNIXSERVER_HPP_

#include <string>
#include "G/net/StreamServer.hpp"

namespace G {

    class UNIXServer: virtual public G::StreamServer
    {
        std::string path;
    protected:
        SOCKET initSocket() override;
    public:
        UNIXServer(G::handle_connect_fn);
        int setPath(const char *);
        int setPath(std::string &);
    };

}

#endif
