#include <iostream>
#include <thread>
#include <semaphore.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include <queue>
#include <vector>
#include <random>
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
	thread_local std::mt19937 rng(std::random_device{}());
	while (true) {
		sem_wait(&empty);
		sem_wait(&mutex);
		std::this_thread::sleep_for(std::chrono::seconds((rng() % 3) + 1));
		requestStructure request;
		counter++;
		request.request_id = counter;
		request.ip_address = "";
		int num = rng() % 10;
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
		sem_post(&empty);
	}	
}

int main() {
	// initialize semaphores
	sem_init(&empty, 0, 10);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	// create producer and consumer threads
	vector<thread> listen_threads;
	for (int i = 0; i < 5; i++) {
		listen_threads.push_back(thread(listen));
	}
	vector<thread> request_threads;
	for (int i = 0; i < 5; i++) {
		request_threads.push_back(thread(do_request));
	}
	for (auto& thread : listen_threads) {
		thread.join();
	}
	for (auto& thread : request_threads) {
		thread.join();
	}
	// destroy semaphores
	sem_destroy(&empty);
	sem_destroy(&full);
	sem_destroy(&mutex);
	return 0;
}