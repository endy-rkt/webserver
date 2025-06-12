/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 03:22:51 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/12 03:24:09 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP

#define RESPONSE_HPP

#define HEADER "HTTP/1.0 200 Bad Request\nServer: AkamaiGHost\nMime-Version: 1.0\nContent-Type: text/html\nContent-Length: 312\nExpires: Thu, 12 Jun 2025 07:47:23 GMT\nDate: Thu, 12 Jun 2025 07:47:23 GMT\nConnection: close\n\n"

#define BODY "<HTML><HEAD>\n<TITLE>Invalid URL</TITLE>\n</HEAD><BODY>\n<H1>Invalid URL</H1>\nThe requested URL "", is invalid.<p>\nReference&#32;&#35;9&#46;5ca33017&#46;1749714443&#46;8e39297a\n<P>https&#58;&#47;&#47;errors&#46;edgesuite&#46;net&#47;9&#46;5ca33017&#46;1749714443&#46;8e39297a</P>\n</BODY></HTML>\n"

#endif