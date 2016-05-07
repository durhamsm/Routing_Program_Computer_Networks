/*
 * Advert.h
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

#ifndef ADVERT_H_
#define ADVERT_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sstream>
#include "GlobalVars.h"
#include <fstream>
#include <vector>
#include <map>
#include <string.h>
#include "RoutingTable.h"

class Advert {
public:

	static RoutingTable last_table_sent;

	Advert();
	virtual ~Advert();

	static void setup_out_addresses(std::vector<std::string> addresses);
	static void send_advert(RoutingTable);

private:

	static std::map <std::string, addrinfo> address_map;
	static int sockfd;

	static std::string get_advert_text(RoutingTable routing_table, std::string dest_addr);
	static std::vector<std::string> neighbors;
	static void setup_address(std::string);
	static void setup_socket();
	static void set_last_table_sent(RoutingTable routing_table);
	static void print_all(std::vector<std::string> advert_addresses, std::vector<std::string> advert_strings);

};

#endif /* ADVERT_H_ */
