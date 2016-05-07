/*
 * ReceivedAdvert.h
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

#ifndef RECEIVEDADVERT_H_
#define RECEIVEDADVERT_H_

#include "Advert.h"
#include <string>
#include <unistd.h>
//class Advert;

class ReceivedAdvert {

public:

	RoutingTable routing_table;
	std::string source;

	ReceivedAdvert();
	virtual ~ReceivedAdvert();

	static void setup_socket();

private:

	static int rec_sockfd;

	std::string get_ip_address(sockaddr_storage their_addr);
	void fill_routing_table(std::string);
	void receive();
};

#endif /* RECEIVEDADVERT_H_ */
