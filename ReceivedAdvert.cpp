/*
 * ReceivedAdvert.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

//This method is used to stored the information that the host received from its neighbors.
//These received adverts

//When a ReceivedAdvert instance is instantiated, it waits to receive a UDP message and then
//uses that info to initialize the instance.

#include "ReceivedAdvert.h"
using namespace std;

int ReceivedAdvert::rec_sockfd;

ReceivedAdvert::ReceivedAdvert() {
	receive();
}

ReceivedAdvert::~ReceivedAdvert() {
	// TODO Auto-generated destructor stub
}

//The text of the UDP message is used to initialize the RoutingTable that this ReceivedAdvert instance contains.
void ReceivedAdvert::fill_routing_table(string received_string) {
	std::string dest_addr;
	std::string cost_string;
	stringstream received_stream(received_string);

	routing_table[source].destination = source;
	routing_table[source].set_cost(0);

	while (getline(received_stream, dest_addr)) {
		getline(received_stream, cost_string);

		routing_table[dest_addr].destination = dest_addr;
		routing_table[dest_addr].set_cost(atoi(cost_string.c_str()));
	}

}

void ReceivedAdvert::setup_socket() {
	struct addrinfo *res;
	struct addrinfo hints = GlobalVars::HINTS;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, GlobalVars::PORT_NUM, &hints, &res);
	rec_sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	bind(rec_sockfd, res->ai_addr, res->ai_addrlen);
}

void ReceivedAdvert::receive() {

	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof their_addr;
	int MAXRECSIZE = 1500;
	char* rec_buf = new char[MAXRECSIZE]();

	recvfrom(rec_sockfd, rec_buf, MAXRECSIZE, 0, (struct sockaddr *) &their_addr, &addr_size);

	source = get_ip_address(their_addr);

	routing_table = RoutingTable(string(rec_buf));
}

//This will retrieve the IP address of the sender (from whom this host received advert).
string ReceivedAdvert::get_ip_address(sockaddr_storage their_addr) {

	socklen_t addr_size = sizeof their_addr;

	char buffer[INET6_ADDRSTRLEN];
	getnameinfo((struct sockaddr*) &their_addr, addr_size, buffer,sizeof(buffer), 0, 0, NI_NUMERICHOST);

	return string(buffer);
}

