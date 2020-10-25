#include "G/net/UNIXServer.hpp"

using namespace G;

UNIXServer::UNIXServer(G::handle_connect_fn handler) : G::StreamServer(handler)
{
    this->isA = "UNIXServer";
}

int UNIXServer::setPath(const char *str)
{
    this->path = str;
    return 0;
}

int UNIXServer::setPath(std::string &str)
{
    this->path = str;
    return 0;
}

SOCKET UNIXServer::initSocket()
{
    // 创建socket
    return UNIXsetup(this->path.c_str());
}
