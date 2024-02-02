#include "../include/Application.hpp"
#include "../include/SocketServer.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sstream>

#define HOST_LISTENING "0.0.0.0"
#define PORT 4000

void Application::run() {
  std::cout << "Running on " << HOST_LISTENING << ":" <<  PORT <<  std::endl;
  SocketServer server = SocketServer(10, [&](const SocketClient&  client,char buff[4096]){
    std::ostringstream message;
    message << client.get_host() << ":" << client.get_port() << " sent:\n" << buff;
    server.broadcast(message.str().c_str() );
  });
  server.listen(HOST_LISTENING, PORT);
}