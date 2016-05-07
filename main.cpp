//============================================================================
// Name        : assignment3.cpp
// Author      : Sam Durham
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <cstdlib>
#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <string.h>

#include "GlobalVars.h"
#include "GraphTable.h"
#include "RouteEntry.h"
#include "Utilities.h"


using namespace std;

//Vector is declared here so that it is visible to both the receiving thread and main thread.
vector<ReceivedAdvert> received_adverts;

//Graph table is instantiated as part of this initialize method. Also, check to see if config_file,
//was found, and exit/report error if it was not found.
GraphTable initialize(char* config_filename) {
	ifstream config_file;
	config_file.open(config_filename);

	if (!config_file.good()) {
		cout << "Error!! Config file '" << config_filename << "' does not exist" << endl;
		exit(0);
	}
	return GraphTable(&config_file);
}

//Receiving thread will run in parallel to main thread.
void* receive_adverts(void *args) {

	ReceivedAdvert::setup_socket();

	while (true) {
		ReceivedAdvert received_advert;

		pthread_mutex_lock(&GlobalVars::RECEIVED_ADVERTS_LOCK);
		received_adverts.push_back(received_advert);
		pthread_mutex_unlock(&GlobalVars::RECEIVED_ADVERTS_LOCK);
	}

	return NULL;
}


int main(int argc, char* argv[]) {

	pthread_t rec_thread;
	int start_time = get_time_now();

	char* config_filename = argv[1];
	GlobalVars::initialize(argv);

	GraphTable graph_table = initialize(config_filename);

	Advert::setup_out_addresses(graph_table.neighbors);

	pthread_create(&rec_thread, NULL, receive_adverts, NULL);

	//Check to see if any adverts have already been received and are in queue.
	graph_table.update(&received_adverts, true);
	graph_table.routing_tables[GlobalVars::MY_IP].print_detailed_table();
	//graph_table.print();

	Advert::send_advert(graph_table.routing_tables[GlobalVars::MY_IP]);

	//double line will be printed above and below each update, for readability.
	print_advert_divider();

	while (true) {
		usleep(GlobalVars::PERIOD*1e6);

		print_elapsed_time_from_start(start_time);

		graph_table.decrement_all_ttl(30);
		graph_table.update(&received_adverts, false);
		graph_table.routing_tables[GlobalVars::MY_IP].print_detailed_table();
		//graph_table.print();

		Advert::send_advert(graph_table.routing_tables[GlobalVars::MY_IP]);

		print_advert_divider();
	}

	return 0;
}
