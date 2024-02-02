#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include "Socket.hpp"

class SocketClient : public Socket
{
public:
    SocketClient() = default;
    SocketClient(FileDescriptor server_descriptor);
    void disconnect();
    void set_descriptor(FileDescriptor desc) { 
        m_file_descriptor = desc;
    };
    sockaddr_in & get_address_ref() {
        return m_address;
    };
    unsigned long & get_address_size_ref() {
        return m_address_size;
    };
};

#endif