#include "G/net/StreamSocket.hpp"

void G::StreamSocket::init(G::EventListener *listener)
{
    G::StreamSocket::listener = listener;
}

void G::StreamSocket::handle(G::StreamSocket *socket)
{
    G::StreamSocket::handle(socket, EV_IN);
}

void G::StreamSocket::handle(G::StreamSocket *socket, G::event_type_t eventType)
{
    G::Event event;

    event.ident = socket->fd;
    event.event_type = eventType;
    event.context = socket;
    event.function = socket->onData;
    G::StreamSocket::listener->emit(OPT_ADD, event);
}

G::StreamSocket::StreamSocket(SOCKET fd)
{
    this->fd = fd;
}

const ssize_t G::StreamSocket::read(char *buf, const size_t len)
{
    const size_t recv_len = recv(this->fd, buf, len, 0);
    if (-1 != recv_len)
        G::StreamSocket::handle(this, EV_IN);
    return recv_len;
}

const ssize_t G::StreamSocket::send(const char *buf, const int len)
{
    const ssize_t sent_len = ::send(this->fd, buf, len, 0);
    G::StreamSocket::handle(this, EV_OUT);
    return sent_len;
}

void G::StreamSocket::shutdown(int how)
{
    ::shutdown(this->fd, how);
}

void G::StreamSocket::close()
{
    ::shutdown(this->fd, SHUT_RDWR);
    ::close(this->fd);
}
