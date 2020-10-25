#ifndef _STREAM_SOCKET_HPP_
#define _STREAM_SOCKET_HPP_

extern "C" {
    #include "G/net/Gnet.h"
}
#include "G/event/EventListener.hpp"

namespace G
{
    class StreamSocket
    {
    private:
        SOCKET fd;
        static G::EventListener *listener;
        static void handle(G::StreamSocket *, G::event_type_t);
    public:
        static void init(G::EventListener *);
        static void handle(G::StreamSocket *);

        StreamSocket(SOCKET);

        const ssize_t read(char *, const size_t);
        const ssize_t send(const char *, const int);
        void shutdown(int);
        void close();

        virtual void onData();
        virtual void onSent();
        virtual void onClose();
    };
}

#endif
