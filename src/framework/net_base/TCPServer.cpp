#include "G/net/TCPServer.hpp"

/*
#include "openssl/rsa.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
*/

G::TCPServer::TCPServer(unsigned short port, G::handle_connect_fn handler) : G::StreamServer(handler)
{
    this->isA = "TCPServer";
    this->setPort(port);
}

int G::TCPServer::setCert(const char *certFile)
{
    if (nullptr == certFile)
    {
        return -1;
    }
    return 0;
}

int G::TCPServer::setKey(const char *keyFile)
{
    if (nullptr == keyFile)
    {
        return -1;
    }
    return 0;
}

int G::TCPServer::setPort(unsigned short port)
{
    this->port = port;
    return port;
}

SOCKET G::TCPServer::initSocket()
{
    // 创建socket
    return TCPsetup(this->port);
}
