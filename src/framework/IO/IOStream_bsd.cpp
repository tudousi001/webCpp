extern "C"
{
    #include <errno.h>
    #include <sys/socket.h>
}
#include "G/io/IOStream.hpp"
#ifdef __BSD__

static ssize_t putout(int fd, G::FdType type, const char *buf, ssize_t len)
{
    switch (type)
    {
        case G::FdType::FD_SOCKET:
            return send(fd, buf, len, 0);
        default:
            return write(fd, buf, len);
    }
}

inline int min(int foo, int bar)
{
    return foo < bar ? foo : bar;
}

G::IOStream::IOStream(G::EventListener *listener, G::IOHandler *handler)
{
    this->listener = listener;
    this->handler = handler;
    i_event.ident = -1;
    o_event.ident = -1;
}

G::IOStream::~IOStream()
{
    if (-1 != i_event.ident)
    {
        listener->emit(OPT_DEL, &i_event);
    }
    if (-1 != o_event.ident)
    {
        listener->emit(OPT_DEL, &o_event);
    }
}

void G::IOStream::setFd(int fd, G::FdType type)
{
    this->type = type;

    i_event.ident = fd;
    i_event.event_type = EV_IN;
    i_event.context = this;
    i_event.function = G::IOStream::onData;

    o_event.ident = fd;
    o_event.event_type = EV_OUT;
    o_event.context = this;
    o_event.function = G::IOStream::onData;

    listener->emit(OPT_ADD, &i_event);
}

ssize_t G::IOStream::read(char *buf, ssize_t size)
{
    ssize_t siz;

    siz = min(i_event.buf_size, size);
    switch (type)
    {
        case G::FdType::FD_SOCKET:
            return recv(i_event.ident, buf, siz, 0);
        default:
            return ::read(i_event.ident, buf, siz);
    }
}

void G::IOStream::write(std::string &str)
{
    this->writeBuf = str;
    o_event.event_type = EV_OUT;
    listener->emit(OPT_ADD, &o_event);
}

// 静态
void G::IOStream::onData(G::Event &ev)
{
    int fd;
    ssize_t siz, len;
    G::event_type_t event_type;
    G::IOStream *ioStream;

    ioStream = (G::IOStream *)(ev.context);
    fd = ev.ident;

    switch (ev.event_type)
    {
        case EV_ERR:
            ioStream->handler->onError(ioStream, 0);
            break;
        case EV_IN:
            ioStream->handler->onData(ioStream);
            break;
        case EV_OUT:
            // 可写数据量
            siz = min(ioStream->writeBuf.length(), ev.buf_size);
            // 写入
            len = putout(fd, ioStream->type, ioStream->writeBuf.c_str(), siz);
            // 写失败
            if (-1 == len)
            {
                ioStream->handler->onError(ioStream, errno);
                break;
            }
            // 没写完
            if (len < ioStream->writeBuf.length())
            {
                ioStream->writeBuf.erase(0, len);
            }
            // 全部完成
            else
            {
                ioStream->listener->emit(OPT_DEL, &ioStream->o_event);
                ioStream->handler->onWritten(ioStream);
            }
            break;
        default:
            break;
    }
}

#endif
