/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:43:43 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/12 03:04:16 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/*canonic*/
Webserver::~Webserver(void)
{
    if (this->serverSock != -1)
        close(this->serverSock);
}

Webserver::Webserver(void)
{
    this->serverSock = -1;
    this->port = PORT;
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    if (!this->initSock())
    {
        this->setupServAddr(ADDR);

        if (bind(this->serverSock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            this->serverSock = -1;    
        }
    } 
}

Webserver::Webserver(int port, std::string addr):port(port)
{
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    if (!this->initSock())
    {
        this->setupServAddr(addr);

        if (bind(this->serverSock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            this->serverSock = -1;    
        }
    }
}

/*canonic*/

int Webserver::initSock()
{
    int _optval;
    int serverSock;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    this->serverSock = serverSock;

    /*set option to reuse used port*/
    _optval = 1;
    if (setsockopt(this->serverSock, SOL_SOCKET, SO_REUSEADDR, &_optval, sizeof(_optval)) < 0)
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
    if (listen(this->serverSock, LISTENQ) < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "Listening on " << inet_ntoa(this->serverAddr.sin_addr)  << ":" << ntohs(this->serverAddr.sin_port) << "..." << std::endl;
    return (0);
}

int Webserver::acceptConnection(void)
{
    int                 clientSock;
    socklen_t           clientAddrLen;
    struct sockaddr_in  clientAddr;

    clientAddrLen = sizeof(clientAddr);
    clientSock = accept(this->serverSock, (struct sockaddr*)(&clientAddr), &clientAddrLen);
    if (clientSock < 0)
    {
        std::cout << strerror(errno) << std::endl;
        // return (1);
        return (-1);
    }
    /*1337*/
    std::cout << "Client " << inet_ntoa(clientAddr.sin_addr) << " connected using " << ntohs(clientAddr.sin_port) << std::endl; 
    // close(clientSock);
    // return (0);
    return (clientSock);
}

int Webserver::closeServer(void)
{
    if (this->serverSock != -1)
        close(this->serverSock);
    this->serverSock = -1;
    std::cout << "Server closed successfully!" << std::endl;
    return (0);
}

int Webserver::serverRun(void)
{
    if (!this->serverListen())
    {
        this->pollingLoop();
    }
    return (0);
}

std::string Webserver::pollRequest(int clientSock)
{
    int         rstLen;
    std::string request;
    char        buffer[RECV_BUFFER];

    rstLen = 0;
    request = "";
    do
    {
        rstLen = recv(clientSock, buffer, RECV_BUFFER - 1, 0);
        if (rstLen < 0)
        {
            std::cout << strerror(errno) << std::endl;
            break;
        }
        buffer[rstLen] = 0;
        request += buffer;
        if (request.find(CSRF) != std::string::npos)
            break;
    } while (rstLen);
    return (request);
}

int Webserver::sendResponse(const std::string &request, int clientSock)
{
    int         status;
    int         bufLen;
    std::string response;
    char        buffer[RECV_BUFFER];


    status = 0;
    std::cout << "Request:" << std::endl;
    std::cout << request;
    
    /*send response*/
    response = HEADER;
    response += BODY;
    bufLen = response.size();
    strcpy(buffer, response.c_str());
    buffer[bufLen] = 0;
    status = send(clientSock, buffer, bufLen, 0);
    if (status < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    buffer[status] = 0;
    return (0);
}

int Webserver::pollingLoop(void)
{
    int         clientSock;
    std::string request;

    clientSock = -1;
    for (;;)
    {
        clientSock = this->acceptConnection();
        if (clientSock != -1)
        {
            request = this->pollRequest(clientSock);
            sendResponse(request, clientSock);
            close(clientSock);
        }
    }
    return (0);
}
