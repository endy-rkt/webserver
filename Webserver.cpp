/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:43:43 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/05 03:45:19 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/*canonic*/
Webserver::~Webserver(void)
{
    if (this->connSock != -1)
        close(this->connSock);
}

Webserver::Webserver(void)
{
    this->connSock = -1;
    this->port = PORT;
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    if (!this->initSock())
    {
        this->setupServAddr(ADDR);

        if (bind(this->connSock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            this->connSock = -1;    
        }
    } 
}

Webserver::Webserver(int port, std::string addr):port(port)
{
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    if (!this->initSock())
    {
        this->setupServAddr(addr);

        if (bind(this->connSock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            this->connSock = -1;    
        }
    }
}

/*canonic*/

int Webserver::initSock()
{
    int _optval;
    int connSock;

    connSock = socket(AF_INET, SOCK_STREAM, 0);
    if (connSock < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    this->connSock = connSock;

    /*set option to reuse used port*/
    _optval = 1;
    if (setsockopt(this->connSock, SOL_SOCKET, SO_REUSEADDR, &_optval, sizeof(_optval)) < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    return (0);
}

int Webserver::setupServAddr(std::string addr)
{
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->port);
    // this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    return (0);
}

int Webserver::serverListen()
{
    if (listen(this->connSock, LISTENQ) < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "Listening on " << inet_ntoa(this->serverAddr.sin_addr)  << ":" << ntohs(this->serverAddr.sin_port) << "..." << std::endl;
    return (0);
}

int Webserver::acceptConnection(void)
{
    int                 commSock;
    socklen_t           clientAddrLen;
    struct sockaddr_in  clientAddr;

    clientAddrLen = sizeof(clientAddr);
    commSock = accept(this->connSock, (struct sockaddr*)(&clientAddr), &clientAddrLen);
    if (commSock < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    /*1337*/
    std::cout << "Client " << inet_ntoa(clientAddr.sin_addr) << " connected using " << ntohs(clientAddr.sin_port) << std::endl; 
    close(commSock);
    return (0);
}

int Webserver::closeServer(void)
{
    if (this->connSock != -1)
        close(this->connSock);
    this->connSock = -1;
    std::cout << "Server closed successfully!" << std::endl;
    return (0);
}

int Webserver::serverRun(void)
{
    if (!this->serverListen())
    {
        this->acceptConnection();
    }
    return (0);
}
