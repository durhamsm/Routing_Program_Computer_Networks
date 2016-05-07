/*
 * GraphTable.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

//This class holds most of the major methods of the program. In my program, there
//is only one instance of Graph table. The initialize method reads the config file and determines
//which nodes are neighbors and which are not neighbors. This neighbor information, along with the current
//TTL for each neighbor, will be used to set the appropriate costs for the routing tables and entries of the graph
//table prior to beginning the Bellman-ford algorithm.

//See recalculate_routing_table method for bellman-ford algorithm.

#include "GraphTable.h"

using namespace std;


GraphTable::GraphTable(ifstream* config_file): GraphTable() {
	initialize(config_file);
}

GraphTable::~GraphTable() {
	// TODO Auto-generated destructor stub
}

//Table can be printed.
void GraphTable::print() {

	int row_seperator_size = (addresses.size() + 1)*20;

	cout << "Graph Table" << endl;
	char* row_seperator = new char[row_seperator_size]();
	fill_n(row_seperator, row_seperator_size-1, '-');

	cout << row_seperator << endl;

	printf("%20s", "");

	for (string header_addr : addresses) {
		printf("%17s | ", header_addr.c_str());
	}

	cout << endl;

	for (string source_addr : addresses) {
		cout << row_seperator << endl;
		printf("%17s | ", source_addr.c_str());
		routing_tables[source_addr].print(addresses);
		cout << endl;
	}
	cout << row_seperator << endl;
	cout << endl;

}

//The is the Bellmen-ford algorithm.
void GraphTable::recalculate_routing_table() {

	string new_next_hop;

	for (unsigned short loop_iter = 1; loop_iter < sizeof(addresses); ++loop_iter) {

		bool is_change = false;
		for (string pi_addr: addresses) {

			for (string dest_addr: addresses) {
				if (pi_addr.compare(dest_addr) == 0) continue;

				int total_distance = get_total_distance(pi_addr, dest_addr);

				if (total_distance < routing_tables[GlobalVars::MY_IP][dest_addr].cost) {
					new_next_hop = routing_tables[GlobalVars::MY_IP][pi_addr].next_hop;
					routing_tables[GlobalVars::MY_IP].update_entry(dest_addr, new_next_hop, total_distance);
					is_change = true;
				}
			}
		}
		//If there was no calculated change in the host routing table after an iteration, then there
		//will not be changes in subsequent iterations. So loop is exited if no change was detected.
		if (!is_change) break;
	}

}

int GraphTable::get_total_distance(std::string pi_addr, std::string dest_addr) {
	int distance_to_pi_addr = routing_tables[GlobalVars::MY_IP][pi_addr].cost;
	int dist_from_pi_to_dest = routing_tables[pi_addr][dest_addr].cost;
	return distance_to_pi_addr + dist_from_pi_to_dest;
}

//Host routing table will be reset based on neighbors and TTL info, prior to running bellman-ford algorithm.
void GraphTable::reset_self_routing_table_from_file() {

	for (string dest_addr: addresses) {

		if (dest_addr.compare(GlobalVars::MY_IP) == 0) {
			routing_tables[GlobalVars::MY_IP].set_as_self(GlobalVars::MY_IP);
			continue;
		}

		bool is_neighbor = find(neighbors.begin(), neighbors.end(), dest_addr) != neighbors.end();
		int ttl = routing_tables[GlobalVars::MY_IP][dest_addr].ttl;

		routing_tables[GlobalVars::MY_IP].set_ttl(dest_addr, ttl);

		if (is_neighbor) {

			if (ttl <= 0) {
				routing_tables[GlobalVars::MY_IP].set_as_unreachable(dest_addr);
			} else {
				routing_tables[GlobalVars::MY_IP].set_as_neighbor(dest_addr);
			}

		} else {
			routing_tables[GlobalVars::MY_IP].set_as_unreachable(dest_addr);
		}

	}

}

//The non-host rows of the graph table are updated with the received adverts and then bellman-ford algorithm is run,
//After which the received_adverts vector is cleared.
void GraphTable::update(vector<ReceivedAdvert>* received_adverts, bool reload_values) {
	pthread_mutex_lock(&GlobalVars::RECEIVED_ADVERTS_LOCK);

	printf("Received Adverts: %d \n\n", received_adverts->size());

	reset_self_routing_table_from_file();

	for (ReceivedAdvert advert : *received_adverts) {
		add_advert(advert);
	}

	recalculate_routing_table();
	received_adverts->clear();

	pthread_mutex_unlock(&GlobalVars::RECEIVED_ADVERTS_LOCK);
}


void GraphTable::add_advert(ReceivedAdvert advert) {
	routing_tables[GlobalVars::MY_IP].set_ttl(advert.source, GlobalVars::DEFAULT_TTL);
	routing_tables[advert.source] = advert.routing_table;
}

void GraphTable::initialize(std::ifstream* config_file) {
	string line;

	addresses.push_back(GlobalVars::MY_IP);
	routing_tables[GlobalVars::MY_IP][GlobalVars::MY_IP].set_cost(0);

	while(getline(*config_file, line)) {
		int space_index = line.find(" ");
		string dest_addr = line.substr(0, space_index);
		bool is_neighbor = line.substr(space_index + 1, line.size()).compare("yes")== 0;

		if(is_neighbor) {
			neighbors.push_back(dest_addr);
		}

		addresses.push_back(dest_addr);

	}

	reset_self_routing_table_from_file();
	initialize_other_routing_tables();
}

void GraphTable::initialize_ttl() {
	for (string neighbor_addr : neighbors) {
		routing_tables[GlobalVars::MY_IP].set_ttl(neighbor_addr, GlobalVars::DEFAULT_TTL);
	}
}

void GraphTable::decrement_all_ttl(int decrement) {
	for (string neighbor_addr : neighbors) {
		routing_tables[GlobalVars::MY_IP].decrement_ttl(neighbor_addr, GlobalVars::PERIOD);

		if (routing_tables[GlobalVars::MY_IP][neighbor_addr].ttl <= 0) {
			routing_tables[neighbor_addr].set_all_costs(GlobalVars::ROUTING_INFINITY);
		}
	}
}

void GraphTable::initialize_other_routing_tables() {

	for (string source_addr : addresses) {
		if (source_addr.compare(GlobalVars::MY_IP) == 0) continue;

		for (string dest_addr : addresses) {
			routing_tables[source_addr].update_entry(dest_addr, GlobalVars::ROUTING_INFINITY);
		}
	}

}



