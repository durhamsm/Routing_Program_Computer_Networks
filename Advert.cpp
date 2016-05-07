/*
 * Advert.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

//This class is full of static members and functions. It is used for sending an advertisement to all of the neighbors
//Initially, the neighbors should be setup (in this case, neighbors are computed within GraphTable class).
//The setup_socket method is called when the neighbors are initialized.
//A static method takes a RoutingTable as an argument, determines the advertisement text that should be send to each neighbor,
//and then sends the advertisement.


#include "Advert.h"


using namespace std;

int Advert::sockfd;
std::map <std::string, addrinfo> Advert::address_map;
vector<string> Advert::neighbors;
RoutingTable Advert::last_table_sent;

Advert::Advert() {
	// TODO Auto-generated constructor stub
}

Advert::~Advert() {
	// TODO Auto-generated destructor stub
}

void Advert::setup_out_addresses(std::vector<std::string> neighbors) {

	Advert::neighbors = neighbors;
	setup_socket();

	for (string addr: neighbors) {
		if (addr.compare(GlobalVars::MY_IP) == 0) continue;

		setup_address(addr);
	}
}

void Advert::send_advert(RoutingTable routing_table) {
	vector<string> advert_addresses;
	vector<string> advert_strings;


	for (string send_addr: neighbors) {
		if (send_addr.compare(GlobalVars::MY_IP) == 0) continue;

		string advert_string = get_advert_text(routing_table, send_addr);

		advert_addresses.push_back(send_addr);
		advert_strings.push_back(advert_string);

		sendto(sockfd, advert_string.c_str(), advert_string.length(), 0, address_map[send_addr].ai_addr, address_map[send_addr].ai_addrlen);
	}
	//print_all(advert_addresses, advert_strings);
	set_last_table_sent(routing_table);
}

void Advert::print_all(vector<string> advert_addresses, vector<string> advert_strings) {

	cout << "Contents of Adverts to Each Neighbor:" << endl;

	for (unsigned advert_index = 0; advert_index < advert_addresses.size(); ++advert_index) {
		cout << "------------------------" << endl;
		cout << "Advert to: " << advert_addresses.at(advert_index).c_str() << endl;
		cout << advert_strings.at(advert_index).c_str() << endl;
		cout << "------------------------" << endl;
	}
}

//did not end up utilizing this method. I was going to use it to determine when routing table converged.
void Advert::set_last_table_sent(RoutingTable routing_table) {
	//string table_status = (last_table_sent == routing_table) ? "stable": "changing";
	//printf("Routing table status: %s\n", table_status.c_str());
	last_table_sent = routing_table;
}

void Advert::setup_address(string addr) {
	struct addrinfo *res;
	struct addrinfo hints = GlobalVars::HINTS;
	getaddrinfo(addr.c_str(), GlobalVars::PORT_NUM, &hints, &res);
	address_map[addr] = *res;
}

void Advert::setup_socket() {
	struct addrinfo *res;
	struct addrinfo hints = GlobalVars::HINTS;
	getaddrinfo(NULL, GlobalVars::PORT_NUM, &hints, &res);
	sockfd = socket(hints.ai_family, hints.ai_socktype, res->ai_protocol);
}

string Advert::get_advert_text(RoutingTable routing_table, string send_addr) {
	stringstream advert_string_stream;

	for (auto entry: routing_table.entries) {
		advert_string_stream << entry.second.destination << endl;
		if (GlobalVars::IS_SPLIT_HORIZON && routing_table[entry.second.destination].next_hop.compare(send_addr) == 0) {
			advert_string_stream << GlobalVars::ROUTING_INFINITY << endl;
		} else {
			advert_string_stream << entry.second.cost << endl;
		}

	}

	return advert_string_stream.str();

}
