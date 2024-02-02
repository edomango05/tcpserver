#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

typedef int FileDescriptor;

class Socket
{
protected:
  FileDescriptor m_file_descriptor = 0;
  sockaddr_in m_address;
  unsigned long m_address_size;

public:
  Socket(){
    m_address_size = sizeof(m_address);
  };
  FileDescriptor get_descriptor() const
  {
    return m_file_descriptor;
  }
};

#endif