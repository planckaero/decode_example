#pragma once
#include <unistd.h>

class UDP {
  public:
    UDP(const int bind_port, const int timeoutmsec);
    ~UDP() { close(fd); };

    int Read(char buf[], const int maxlen);
    int Write(const char buf[], const int len, const std::string addr, const int port);
  private:
    int fd;

    bool bind_socket(const int listen_port, const int timeoutmsec);
};
