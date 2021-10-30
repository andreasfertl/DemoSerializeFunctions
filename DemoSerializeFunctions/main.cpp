// main.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <thread>
#include <vector>
#include "serializeFunctions.h"

namespace {
	//Demonstrating the usage by one instance
	void simulateOneInstance() {
		//instanciate our function serializer object
		seriealizeFunctions serializer;

		//printing one time the ID of the thread of the serializer
		serializer.run([]() {
			std::cout << "Serializer Thread ID: " << std::this_thread::get_id() << std::endl;
			});


	}

	//simulate nrOfThreads which are printing some debug out to the console
	//the output will be printed in order due to the serializiation
	void simulateSeveralPrintThreads(unsigned int nrOfThreads) {
		//instanciate our function serializer object
		seriealizeFunctions serializer;

		//placeholder for all threads
		std::vector<std::thread> manyThreads;

		//spin up all our nrOfThreads
		for (auto threadcount(0u); threadcount < nrOfThreads; threadcount++) {
			//start a single thread
			manyThreads.emplace_back([&serializer]() {
				//read the id of this thread we just started
				auto id = std::this_thread::get_id();
				//and utilize our serializer to output our data in order
				serializer.run([id]() {
					std::cout << "serialized Thread ID: " << id << std::endl;
					});
				});
		}

		//join (wait for finish) our threads to exit gracefully
		for (auto& thread : manyThreads) {
			thread.join();
		}
	}


	//simulate nrOfThreads which are printing some debug out to the console
	//the output will be printed NOT in order since it is not serialized!
	void simulateSeveralPrintThreadsUnorderd(unsigned int nrOfThreads) {

		//instanciate our function serializer object
		seriealizeFunctions serializer;

		//placeholder for all threads
		std::vector<std::thread> manyThreads;

		//spin up all our nrOfThreads
		for (auto threadcount(0u); threadcount < nrOfThreads; threadcount++) {
			//start a single thread
			manyThreads.emplace_back([&serializer]() {
				std::cout << "unorderd Thread ID: " << std::this_thread::get_id() << std::endl;
				});
		}

		//join (wait for finish) our threads to exit gracefully
		for (auto& thread : manyThreads) {
			thread.join();
		}
	}
}


int main()
{
	std::cout << "The Main Thread ID: " << std::this_thread::get_id() << std::endl;

	//serialize "one" instance
	simulateOneInstance();

	//uncomment to see an example of the problem 
	//simulateSeveralPrintThreadsUnorderd(30u);

	//serialize nrOfThreads instances
	simulateSeveralPrintThreads(30u);



	return 0;
}
