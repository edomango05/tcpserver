#include "../include/Application.hpp"
#include "../include/SocketServer.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>

#define HOST_LISTENING "0.0.0.0"
#define PORT 4000

void Application::run() {
  std::cout << "running on port " << PORT <<  std::endl;
  SocketServer server = SocketServer(10, [&](const SocketClient&  client,char buff[4096]){
    std::string message = std::string(client.get_host())+ ":" + std::to_string(client.get_port()) + " sent:\n" + std::string(buff) ;
    server.broadcast(message.c_str() );
  });
  server.listen(HOST_LISTENING, PORT);
}