#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include "Socket.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>

class SocketClient : public Socket
{
public:
    SocketClient() = default;
    SocketClient(FileDescriptor server_descriptor);
    void disconnect();
    void set_descriptor(FileDescriptor desc) { 
        m_file_descriptor = desc;
    };

    char * get_host() const{
        return inet_ntoa(m_address.sin_addr);
    };

    uint16_t get_port() const{
        return ntohs(m_address.sin_port);
    };

    auto send(const char *buff) const{
        ::send(m_file_descriptor, buff, strlen(buff), 0);
    };

    sockaddr_in* get_address_ptr() {
        return &m_address;
    };
    unsigned long * get_address_size_ptr() {
        return &m_address_size;
    };
};

#endif