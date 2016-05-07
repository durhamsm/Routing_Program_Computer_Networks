/*
 * GlobalVars.cpp
 *
 *  Created on: Nov 25, 2015
 *      Author: durhamsm
 */

//This class holds only static members and functions that are needed throughout various
//classes. The majority of it's members are taken from the command line arguments, but
//mutex and common network/socket info is also stored here.

#include "GlobalVars.h"

using namespace std;

char* GlobalVars::PORT_NUM;
int GlobalVars::DEFAULT_TTL;
int GlobalVars::ROUTING_INFINITY;
int GlobalVars::PERIOD;
bool GlobalVars::IS_SPLIT_HORIZON;
string GlobalVars::MY_IP;
pthread_mutex_t GlobalVars::RECEIVED_ADVERTS_LOCK;

struct addrinfo GlobalVars::HINTS;

GlobalVars::GlobalVars() {

}

GlobalVars::~GlobalVars() {

}

void GlobalVars::initialize(char* argv[]) {
	initialize_args(argv);
	initialize_my_ip();
	initialize_mutex();
	initialize_socket_info();
}

void GlobalVars::initialize_my_ip() {
	MY_IP = get_my_ip();
}

//This was the best method that I could come up with for determining the IP address of the host.
string GlobalVars::get_my_ip() {
	FILE *ls = popen("wget http://ipinfo.io/ip -qO -", "r");
	char buf[256];
	fgets(buf, sizeof(buf), ls);
	pclose(ls);
	string ip_string(buf);
	ip_string.erase(std::remove(ip_string.begin(), ip_string.end(), '\n'), ip_string.end());
	return ip_string;
}

void GlobalVars::initialize_args(char* argv[]) {
	PORT_NUM = argv[2];
	DEFAULT_TTL = atoi(argv[3]);
	ROUTING_INFINITY = atoi(argv[4]);
	PERIOD = atoi(argv[5]);
	IS_SPLIT_HORIZON = strcmp(argv[6], "true") == 0;
}

void GlobalVars::initialize_mutex() {
	RECEIVED_ADVERTS_LOCK = PTHREAD_MUTEX_INITIALIZER;
}

void GlobalVars::initialize_socket_info() {
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	HINTS = hints;
}

