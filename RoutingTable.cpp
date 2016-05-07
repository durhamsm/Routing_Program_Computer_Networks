/*
 * RoutingTable.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

#include "RoutingTable.h"
#include <vector>
using namespace std;

RoutingTable::RoutingTable() {
	// TODO Auto-generated constructor stub
}

//The routing table can be instantiated by passing in a received text as the argument.
RoutingTable::RoutingTable(std::string table_string) {

	stringstream table_stream(table_string);
	string dest_addr;
	string cost;

	entries_type new_entries;

	while (getline(table_stream, dest_addr)) {
		getline(table_stream, cost);
		update_entry(dest_addr, atoi(cost.c_str()));
	}
}


RoutingTable::~RoutingTable() {
	// TODO Auto-generated destructor stub
}

void RoutingTable::update_entry(std::string dest_addr, int new_cost) {
	entries[dest_addr].cost = new_cost;
}

void RoutingTable::update_entry(std::string dest_addr, std::string new_next_hop, int new_cost) {
	entries[dest_addr].next_hop = new_next_hop;
	entries[dest_addr].cost = new_cost;
}

void RoutingTable::set_as_neighbor(std::string dest_addr) {
	entries[dest_addr].destination = dest_addr;
	entries[dest_addr].next_hop = dest_addr;
	entries[dest_addr].cost = 1;
}

void RoutingTable::set_as_unreachable(std::string dest_addr) {
	entries[dest_addr].destination = dest_addr;
	entries[dest_addr].next_hop = "NULL";
	entries[dest_addr].cost = GlobalVars::ROUTING_INFINITY;
}

void RoutingTable::set_as_self(std::string dest_addr) {
	entries[dest_addr].destination = dest_addr;
	entries[dest_addr].next_hop = dest_addr;
	entries[dest_addr].cost = 0;
}


RouteEntry RoutingTable::operator [](string addr) {
		return entries[addr];
}

void RoutingTable::print(vector<string> dest_addresses) {
	for (string dest_addr : dest_addresses) {
		printf("%17d | ", entries[dest_addr].cost);
	}
}

void RoutingTable::set_ttl(string dest_addr, int new_ttl) {
	entries[dest_addr].set_ttl(new_ttl);
}

//Decrement TTL, but don't let it go below zero.
void RoutingTable::decrement_ttl(string dest_addr, int decrement) {
	int new_ttl = entries[dest_addr].ttl - decrement;
	new_ttl = new_ttl < 0 ? 0 : new_ttl;
	entries[dest_addr].set_ttl(new_ttl);
}

//This will set the cost of all destinations of the routing table to a new value (usually used to set to INFINITY).
void RoutingTable::set_all_costs(int new_cost) {
	for (auto& entry : entries) {
		entry.second.set_cost(new_cost);
	}
}

void RoutingTable::print_detailed_table() {

	cout << "Routing Table" << endl;

	int row_seperator_size = 56;
	char* row_seperator = new char[row_seperator_size]();
	fill_n(row_seperator, row_seperator_size-1, '-');

	cout << row_seperator << endl;
	RouteEntry::print_detailed_entry_header();
	cout << row_seperator << endl;

	for (auto entry: entries) {
		entry.second.print_detailed_entry();
		cout << endl << row_seperator << endl;
	}

}




