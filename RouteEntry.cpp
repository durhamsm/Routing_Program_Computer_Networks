/*
 * RouteEntry.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: durhamsm
 */

#include "RouteEntry.h"
using namespace std;


//RouteEntry& operator= (RouteEntry&& new_route_entry) {
//	return *this;
//}

RouteEntry::~RouteEntry() {
	// TODO Auto-generated destructor stub
}

void RouteEntry::set_cost(int new_cost) {
	cost = new_cost;
}

void RouteEntry::set_ttl(int new_ttl) {
	ttl = new_ttl;
}

void RouteEntry::set_next_hop(string new_next_hop) {
	next_hop = new_next_hop;
}

void RouteEntry::print_detailed_entry() {
	printf("%17s | ", destination.c_str());
	printf("%17s | ", next_hop.c_str());
	printf("%5d | ", cost);
	printf("%5d | ", ttl);
}

void RouteEntry::print_detailed_entry_header() {
	printf("%17s | %17s | %5s | %5s | \n", "Node", "Next Hop", "Cost", "TTL");
}

bool RouteEntry::operator==(RouteEntry& rhs) {
	return true;
}
