/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:59:14 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/16 10:25:32 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP

#define HEADER_HPP

#include "response.hpp"

#include <iostream>

#include <string>

#include <cstdlib>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <poll.h>

#include <unistd.h>

#include <string.h>

#include <errno.h>

#include <limits.h>

#define PORT 8080

#define ADDR "0.0.0.0"

#define LISTENQ 10

#define RECV_BUFFER 1024

#define CSRF "\r\n\r\n"

#endif