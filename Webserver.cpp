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

Webserver::Webserver(void)
{
    this->connSock = -1;
    this->port = PORT;
    bzero(&this->serverAddr, sizeof(this->serverAddr));
}

int Webserver::initSock()
{
    int _optval;
    int connSock;

    connSock = socket(AF_INET, SOCK_STREAM, 0);
    if (connSock < 0)
    {
        perror("socket error");
        return (1);
    }
    this->connSock = connSock;

    _optval = 1;
    if (setsockopt(this->connSock, SOL_SOCKET, SO_REUSEADDR, &_optval, sizeof(_optval)) < 0)
    {
        perror("setsockopt error");
        return (1);
    }
    return (0);
}

int Webserver::setupServAddr()
{
    bzero(&this->serverAddr, sizeof(this->serverAddr));
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->port);
    this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    return (0);
}

int Webserver::serverListen()
{
    if (!this->initSock())
    {
        this->setupServAddr();

        if (bind(this->connSock, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
        {
            perror("bind error");
            return (1);    
        }

        if (listen(this->connSock, LISTENQ) < 0)
        {
            perror("listen error");
            return (1);
        }
        std::cout << "Listening on 0.0.0.0:" << PORT << "..." << std::endl;
        return (0);
    }
    std::cout << "Can`t initiate connection socket..." << std::endl;
    return (1);
}

