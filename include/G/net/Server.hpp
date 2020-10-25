#ifndef _GSERVER_HPP_
#define _GSERVER_HPP_

extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <errno.h>
    #ifdef __BSD__
        #include <sys/event.h>
    #endif
}

namespace G {
    class Server
    {
    protected:
        virtual SOCKET initSocket() =0;
    public:
        virtual int service(int) =0;
    };
}
#endif
