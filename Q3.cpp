#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
#include <string>
#include <cstdlib>
using namespace std;

struct requestStructure {
	int request_id;
	string ip_address;
	string page_requested;
};

queue<requestStructure> msg_queue;

string webPages[10] = {"google.com", "yahoo.com", "youtube.com", "amazon.com", "facebook.com", "instagram.com", "github.com", "gmail.com", "outlook.com", "chess.com"};

int shared = 0;

void listen() {
	sleep_for(seconds((rand() % 3) + 1));
	requestStructure request;
	shared++;
	request.request_id = shared;
	request.ip_address = "";
	request.page_requested = webPages[rand() % 10]
}

void do_request() {
	
}

int main() {
	
	return 0;
}