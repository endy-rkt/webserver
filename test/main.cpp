/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trazanad <trazanad@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 01:58:46 by trazanad          #+#    #+#             */
/*   Updated: 2025/06/16 09:36:42 by trazanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

int	main(void)
{
	Webserver	server = Webserver(8080, "0.0.0.0");

	
	server.serverRun();
	server.closeServer();
	return (0);
}