/*
 * GraphTable.h
 *
 *  Created on: Nov 14, 2015
 *      Author: durhamsm
 */



#ifndef GRAPHTABLE_H_
#define GRAPHTABLE_H_

#include "GlobalVars.h"
#include "ReceivedAdvert.h"
#include "RoutingTable.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>



typedef std::vector<std::string> addresses_type;

class GraphTable {

public:

	addresses_type addresses;
	addresses_type neighbors;

	std::map <std::string, RoutingTable> routing_tables;

	GraphTable(){};
	GraphTable(std::ifstream*);
	virtual ~GraphTable();

	void update(std::vector<ReceivedAdvert>*, bool);
	void print();
	void decrement_all_ttl(int);

private:
	static std::string source;

	void initialize(std::ifstream*);
	void recalculate_routing_table();
	int get_total_distance(std::string, std::string);
	void initialize_ttl();
	void reset_self_routing_table_from_file();
	void initialize_other_routing_tables();
	void add_advert(ReceivedAdvert advert);
	void calculate_next_hop();
};

#endif /* GRAPHTABLE_H_ */
