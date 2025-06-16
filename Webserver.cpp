/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:43:43 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/16 11:16:40 by trazanad         ###   ########.fr       */
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
        return (-1);
    }
    /*1337*/
    std::cout << "Client " << inet_ntoa(clientAddr.sin_addr) << " connected using " << ntohs(clientAddr.sin_port) << std::endl; 
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
        this->serverLoop();
    }
    return (0);
}

std::string Webserver::getRequest(int clientSock)
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

int Webserver::formatResponse(char *buffer)
{
    int         bufLen;
    std::string response;

    response = HEADER;
    response += BODY;
    bufLen = response.size();

    strncpy(buffer, response.c_str(), RECV_BUFFER);
    buffer[RECV_BUFFER - 1] = 0;
    if (bufLen < RECV_BUFFER)
        buffer[bufLen] = 0;
    else
        bufLen = RECV_BUFFER;
    
    return (bufLen);
}

int Webserver::sendResponse(const std::string &request, int clientSock)
{
    int         status;
    int         bufLen;
    char        response[RECV_BUFFER];


    status = 0;
    std::cout << "Request:" << std::endl;
    std::cout << request;
    
    /*send response*/
    bufLen = formatResponse(response);
    status = send(clientSock, response, bufLen, 0);
    if (status < 0)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    response[status] = 0;
    return (0);
}

void    Webserver::initiatePollfd(void)
{
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        this->fds[i].fd = -1;
    }
}

int Webserver::setPollfd(int clientSock, int _events, int *maxIndex)
{    
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        if (this->fds[i].fd < 0)
        { 
            this->fds[i].fd = clientSock;
            this->fds[i].events = _events;
            if (*maxIndex < i)
                *maxIndex = i;
            return (i);
        }
    }
    return (-1);
}

int Webserver::pollingLoop(int maxIndex, int readyNum)
{
    int         clientSock;
    std::string request;

    for (int i = 0; i <= maxIndex; i++)
    {
        clientSock = this->fds[i].fd;
        if (clientSock < -1)
            continue;
        if (this->fds[i].revents & (POLLIN | POLLERR))
        {
            request = this->getRequest(clientSock);
            sendResponse(request, clientSock);
            close(clientSock);
            this->fds[maxIndex].fd = -1;
            readyNum--;
            if (readyNum <= 0)
                break;
        }
    }
    return (0);
}

int Webserver::serverLoop(void)
{
    int             readyNum;
    int             maxIndex;
    int             clientSock;
    std::string     request;

    maxIndex = 0;
    clientSock = -1;
    this->initiatePollfd();
    for (;;)
    {
        clientSock = this->acceptConnection();
        if (clientSock != -1)
        {
            this->setPollfd(clientSock, POLLIN, &maxIndex);
            readyNum = poll(this->fds, maxIndex + 1, -1);
            if ( readyNum < 0)
            {
                std::cout << strerror(errno) << std::endl;
                close(clientSock);
                this->fds[maxIndex].fd = -1;
                return (-1);
            }
            this->pollingLoop(maxIndex, readyNum);
        }
    }
    return (0);
}
