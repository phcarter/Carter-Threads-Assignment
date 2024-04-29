#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include <queue>
using namespace std;

sem_t empty;
sem_t full;
sem_t mutex;

struct requestStructure {
	int request_id;
	string ip_address;
	string page_requested;
};

queue<requestStructure> msg_queue;

string webPages[10] = {"google.com", "yahoo.com", "youtube.com", "amazon.com", "facebook.com", "instagram.com", "github.com", "gmail.com", "outlook.com", "chess.com"};

int counter = 0;

void listen() {
	while (true) {
		sem_wait(&empty);
		sem_wait(&mutex);
		std::this_thread::sleep_for(std::chrono::seconds((rand() % 3) + 1));
		requestStructure request;
		counter++;
		request.request_id = counter;
		request.ip_address = "";
		int num = rand() % 10;
		request.page_requested = webPages[num];
		msg_queue.push(request);
		sem_post(&mutex);
		sem_post(&full);
	}
}

void do_request() {
	while (true) {
		sem_wait(&full);
		sem_wait(&mutex);
		if (msg_queue.empty()) {
			sem_post(&mutex);
			sem_post(&full);
			break;
		}
		requestStructure request = msg_queue.front();
		request = msg_queue.front();
		msg_queue.pop();
		cout << "thread " << std::this_thread::get_id() << " completed request " << request.request_id << " requesting webpage " << request.page_requested << endl;
		sem_post(&mutex);
		sem_post(&full);
	}	
}

int main() {
	// initialize semaphores
	sem_init(&empty, 0, 10);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	// create producer and consumer threads
	thread listen_thread(listen);
	thread request_thread(do_request);
	// wait for threads to finish (should never happen)
	listen_thread.join();
	request_thread.join();
	// destroy semaphores
	sem_destroy(&empty);
	sem_destroy(&full);
	sem_destroy(&mutex);
	return 0;
}