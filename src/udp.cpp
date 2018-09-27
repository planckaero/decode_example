#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "udp.h"

UDP::UDP(const int bind_port, const int timeoutmsec)
{
  if(!bind_socket(bind_port, timeoutmsec))
    throw; 
}

int UDP::Read(char buf[], const int maxlen)
{
  struct sockaddr_in addr;
  socklen_t socklen = sizeof(addr);
  int nrecv = recvfrom(fd, buf, maxlen, 0, (sockaddr*)&addr, &socklen);
  return nrecv;
}

bool UDP::bind_socket(const int listen_port, const int timeoutmsec)
{
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_port = htons(listen_port);
  addr.sin_family = AF_INET;
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  if(fd == -1)
  {
    std::cerr << "Unable to create UDP socket." << std::endl;
    return false;
  }

  if(addr.sin_port==0)
  {
    std::cerr << "Invalid local port." << std::endl;
    return false;
  }

  if(::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
  {
    std::cerr << "UDP port bind failed." << std::endl;
    return false;
  }

  // Set receive timeout
  struct timeval tv;

  if(timeoutmsec >= 1000)
    tv.tv_sec = timeoutmsec / 1000;
  else
    tv.tv_sec = 0;

  tv.tv_usec = (timeoutmsec % 1000) * 1000;

  if(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
  {
    std::cerr << "Error setting socket options." << std::endl;
    return false;
  }

  return true;
}

int UDP::Write(const char buf[], const int len, const std::string address, const int port)
{
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if(inet_pton(AF_INET, address.c_str(), &addr.sin_addr) == 0)
  {
    std::cerr << "Could not translate remote IP address string to in_addr: " << address << std::endl;
    return -1;
  }
  int socklen = sizeof(addr);
  int nret = sendto(fd, buf, len, 0, (const sockaddr*)&addr, socklen);
  if(nret < len)
  {
    std::cerr << "Unable to send all data to UDP endpoint." << std::endl;
  }
  return nret;
}
