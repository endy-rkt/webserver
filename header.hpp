#ifndef HEADER_HPP

#define HEADER_HPP

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <unistd.h>

int runServer();
int runClient();

#endif