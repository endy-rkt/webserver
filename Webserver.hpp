/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 02:02:17 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/05 03:34:10 by trazanad         ###   ########.fr       */
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
        int connSock;
        struct sockaddr_in serverAddr;
        /*method*/
        int initSock(void);
        int setupServAddr(std::string addr);
        int serverListen(void);
        int acceptConnection(void);
        
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