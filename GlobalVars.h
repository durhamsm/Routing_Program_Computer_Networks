/*
 * GlobalVars.h
 *
 *  Created on: Nov 25, 2015
 *      Author: durhamsm
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>


class GlobalVars {
public:

	static char* PORT_NUM;
	static int DEFAULT_TTL;
	static int ROUTING_INFINITY;
	static int PERIOD;
	static bool IS_SPLIT_HORIZON;
	static std::string MY_IP;
	static pthread_mutex_t RECEIVED_ADVERTS_LOCK;
	static struct addrinfo HINTS;

	GlobalVars();
	virtual ~GlobalVars();

	static void initialize(char* argv[]);

private:
	static void initialize_my_ip();
	static void initialize_args(char* argv[]);
	static void initialize_mutex();
	static void initialize_socket_info();
	static std::string get_my_ip();
};


#endif /* GLOBALVARS_H_ */
