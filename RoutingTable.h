/*
 * RoutingTable.h
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */

#ifndef ROUTINGTABLE_H_
#define ROUTINGTABLE_H_

#include "RouteEntry.h"
#include "GlobalVars.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

typedef std::map <std::string, RouteEntry> entries_type;

class RoutingTable {
public:
	entries_type entries;
	RoutingTable();
	RoutingTable(std::string table_string);
	//bool operator==(const RoutingTable& rhs);

	virtual ~RoutingTable();
	void update_entry(std::string dest_adrr, std::string next_hop_addr, int new_cost);
	void update_entry(std::string dest_addr, int new_cost);
	void print(std::vector<std::string>);
	void set_ttl(std::string dest_addr, int new_ttl);
	void decrement_ttl(std::string dest_addr, int new_ttl);
	void set_all_costs(int new_cost);
	void set_as_neighbor(std::string dest_addr);
	void set_as_unreachable(std::string dest_addr);
	void set_as_self(std::string dest_addr);
	void print_detailed_table();

	RouteEntry operator [](std::string);
};

#endif /* ROUTINGTABLE_H_ */
