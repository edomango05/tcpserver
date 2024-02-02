#include "../include/SocketClient.hpp"
#include <sys/socket.h>
#include <unistd.h>

void SocketClient::disconnect() {
  ::close(m_file_descriptor);
  m_file_descriptor = 0;
}