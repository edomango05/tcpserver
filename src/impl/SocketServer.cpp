#include "../include/SocketServer.hpp"
#include "../include/Exceptions.hpp"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstring>
#include <exception>
#include <iostream>
#include <string_view>
#include <unistd.h>

SocketServer::SocketServer(
    uint _max_sockets_connected,
    std::function<void(const SocketClient &, char[4096])> _on_data) {
  int opt = 1;
  m_max_sockets_connected = _max_sockets_connected;
  m_on_data = _on_data;
  connected_client_sockets = std::vector<SocketClient>(m_max_sockets_connected);
  m_file_descriptor = ::socket(AF_INET, SOCK_STREAM, 0);
  if (::setsockopt(m_file_descriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0) {
    THROW_EXCEPTION_ERRNO;
  }
}

void SocketServer::listen(const char *host, uint16_t port) {
  inet_pton(AF_INET, host, &m_address.sin_addr);
  m_address.sin_port = htons(port);

  if (::bind(m_file_descriptor,
             reinterpret_cast<const struct sockaddr *>(&m_address),
             sizeof(m_address)) < 0) {
    
    THROW_EXCEPTION_ERRNO;
  }
  if (::listen(m_file_descriptor, SOMAXCONN) < 0) {
    THROW_EXCEPTION_ERRNO;
  }
  m_address_size = sizeof(m_address);
  loop();
}
void SocketServer::disconnect() { close(m_file_descriptor); }

void SocketServer::broadcast(const char *buff) {
  for (SocketClient &client : connected_client_sockets) {
    client.send(buff);
  }
}

void SocketServer::loop() {
  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  int result;

  while (true) {
    FD_ZERO(&m_readfds);
    FD_SET(m_file_descriptor, &m_readfds);
    m_max_fd = m_file_descriptor;
    for (SocketClient &client : connected_client_sockets) {
      FileDescriptor fd = client.get_descriptor();
      if (fd > 0) {
        FD_SET(fd, &m_readfds);
      }
      if (fd > m_max_fd) {
        m_max_fd = fd;
      }
    }

    if ((result = ::select(m_max_fd + 1, &m_readfds, NULL, NULL, &tv)) < 0) {
      std::cout << "Select error occured" << std::endl;
    }

    if (FD_ISSET(m_file_descriptor, &m_readfds)) {
      for (SocketClient &client : connected_client_sockets) {
        if (client.get_descriptor() == 0) {
          client.set_descriptor(::accept(
              m_file_descriptor,
              reinterpret_cast<sockaddr *>(client.get_address_ptr()),
              reinterpret_cast<socklen_t *>(client.get_address_size_ptr())));
              auto add = *client.get_address_ptr();
          std::cout << "Host " << std::string(inet_ntoa(add.sin_addr)) << " connected with fd " << client.get_descriptor() << std::endl;
          break;
        }
      }
    }
    for (SocketClient &client : connected_client_sockets) {
      auto sd = client.get_descriptor();
      if (sd != 0 && FD_ISSET(sd, &m_readfds)) {
        memset(m_buffer, 0, 4096);
        ssize_t valread;
        if ((valread = read(sd, m_buffer, 4096)) == 0) {
          std::cout << "Host disconnected" << std::endl;
          client.disconnect();
        } else if (valread == -1) {
        } else {
          // Somebody sent some data
          m_on_data(client, m_buffer);
        }
      }
    }
  }
}
