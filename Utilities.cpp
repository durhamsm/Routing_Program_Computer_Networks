/*
 * Utilities.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: durhamsm
 */

#include "Utilities.h"

using namespace std;


int get_time_now() {
    struct timeval val;
    long seconds;
    gettimeofday(&val, NULL);
    seconds = val.tv_sec;

    return seconds;
}

void print_advert_divider() {
	cout << "================================================" << endl;
}

void print_elapsed_time_from_start(int start_time) {
	printf("Update at %d seconds from start \n", get_time_now() - start_time);
}

