/*
 * RouteEntry.h
 *
 *  Created on: Nov 15, 2015
 *      Author: durhamsm
 */

#ifndef ROUTEENTRY_H_
#define ROUTEENTRY_H_
#include <string>
#include "GlobalVars.h"

class RouteEntry {
public:
	RouteEntry(): ttl(GlobalVars::DEFAULT_TTL), cost(GlobalVars::ROUTING_INFINITY){};
	RouteEntry(std::string dest_addr, int cost_to): destination(dest_addr), cost(cost_to), ttl(GlobalVars::DEFAULT_TTL){};
	bool operator==(RouteEntry& rhs);

	virtual ~RouteEntry();

	std::string destination;
	short ttl;
	int cost;
	std::string next_hop;

	void set_cost(int new_cost);
	void set_ttl(int new_ttl);
	void set_next_hop(std::string new_next_hop);
	void reinitialize();
	void print_detailed_entry();
	static void print_detailed_entry_header();

	RouteEntry(const RouteEntry& x) = default;
	//RouteEntry& operator= (RouteEntry&& new_route_entry) = default;

};

#endif /* ROUTEENTRY_H_ */
