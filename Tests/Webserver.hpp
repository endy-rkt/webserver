/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:02:17 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/16 11:04:03 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP

#define WEBSERVER_HPP

#include "header.hpp"

/*
    - create conn socket
    - set up address family and sock
    - bind
    - accept loop 
    - poll loop
*/

class   Webserver
{
    private:
        int port;
        int serverSock;
        std::string      requests[FD_SETSIZE];
        struct  pollfd  fds[FD_SETSIZE];
        struct sockaddr_in serverAddr;
        /*method*/
        int initSock(void);
        int setupServAddr(std::string addr);
        int serverListen(void);
        int acceptConnection(void);
        /*response request*/
        std::string getRequest(int clientSock);
        int formatResponse(char *buffer);
        int sendResponse(const std::string &request, int clientSock);
        /*polling*/
        void    initiatePollfd(void);
        int     setPollfd(int clientSock, int _events, int *maxIndex);
        int     pollingLoop(int maxIndex, int readyNum);
        /**/
        int serverLoop(void);
        
    public:
        Webserver(int port, std::string addr);
        ~Webserver(void);
        Webserver(void);
        Webserver( const Webserver &anotherServ);
        Webserver   &operator=(const Webserver &anotherServ);
        int serverRun(void);
        int closeServer(void);
};

#endif