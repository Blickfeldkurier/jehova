#ifndef NETWORK_H
#define NETWORK_H

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <sstream>
#include <iostream>

#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "canvas.h"

class Network
{
private:
    int port;
    int listenSocket;
    int addrlen;
    int newSocket;
    std::vector<int> clientSocket;
    int maxSd;
    int activity;
    int valread;
    int sd;

    struct sockaddr_in address;
    fd_set readfds;


    void init();
    void processMsg(std::string message, Canvas *canvas);

public:
    Network(int port = 2342);
    void tick(Canvas *canvas);
};

#endif // NETWORK_H
