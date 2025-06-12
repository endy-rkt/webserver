/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:59:14 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/12 02:57:28 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP

#define HEADER_HPP

#include <iostream>

#include <string>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <string.h>

#include <errno.h>

#include <cstdlib>

#define PORT 8080

#define ADDR "0.0.0.0"

#define LISTENQ 10

#define RECV_BUFFER 1024

#define CSRF "\n\r\n"

#define HEADER "HTTP/1.0 200 Bad Request\nServer: AkamaiGHost\nMime-Version: 1.0\nContent-Type: text/html\nContent-Length: 312\nExpires: Thu, 12 Jun 2025 07:47:23 GMT\nDate: Thu, 12 Jun 2025 07:47:23 GMT\nConnection: close\n\n"

#define BODY "<HTML><HEAD>\n<TITLE>Invalid URL</TITLE>\n</HEAD><BODY>\n<H1>Invalid URL</H1>\nThe requested URL "", is invalid.<p>\nReference&#32;&#35;9&#46;5ca33017&#46;1749714443&#46;8e39297a\n<P>https&#58;&#47;&#47;errors&#46;edgesuite&#46;net&#47;9&#46;5ca33017&#46;1749714443&#46;8e39297a</P>\n</BODY></HTML>\n"

#endif