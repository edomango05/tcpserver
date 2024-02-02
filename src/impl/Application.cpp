#include "../include/Application.hpp"
#include "../include/SocketServer.hpp"
#include <iostream>

#define HOST_LISTENING "0.0.0.0"
#define PORT 4000

void Application::run() {
  std::cout << "running on port " << PORT <<  std::endl;
  SocketServer server = SocketServer(10, [](char buff[4096]){
    std::cout  << "data: " << std::string(buff)  << std::endl;
  });
  server.listen(HOST_LISTENING, PORT);
}