#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "Socket.hpp"
#include "SocketClient.hpp"
#include <functional>
#include <vector>

class SocketServer : public Socket
{
private:
  char m_buffer[4096];
  uint m_max_sockets_connected;
  FileDescriptor m_max_fd = 0;
  std::vector<SocketClient> connected_client_sockets;
  std::function<void(char[4096])> m_on_data;

  void loop();
  fd_set m_readfds;

public:
  SocketServer(uint _max_sockets_connected, std::function<void(char[4096])> _on_data);
  void listen(const char *host, uint16_t port);

  void broadcast();
  void send_to_client();
};

#endif