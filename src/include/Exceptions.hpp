#ifndef SOCKET_EXCEPTION_HPP
#define SOCKET_EXCEPTION_HPP

#include <sstream>
#include <stdexcept>

#define THROW_EXCEPTION_ERRNO                                                  \
  throw SocketException(strerror(errno), __FILE__, __LINE__)

struct SocketException : public std::runtime_error {
private:
  std::string msg;

public:
  SocketException(const std::string &arg, const char *file, int line)
      : std::runtime_error(arg) {
    std::ostringstream o;
    o << "An error occured in \033[1m" << file << "\033[0m at line \033[1m"
      << line << "\033[0m : " << arg;
    msg = o.str();
  }
  ~SocketException() throw() {}
  const char *what() const throw() { return msg.c_str(); }
};

#endif