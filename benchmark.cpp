// Author: Prashant Pandey <prashant.pandey@utah.edu>
// For use in CS6968 & CS5968

#include <iostream>
#include <set>
#include <chrono>
#include <openssl/rand.h>
#include "veb.h"
#include <string>

using namespace std::chrono;

double elapsed(high_resolution_clock::time_point t1, high_resolution_clock::time_point t2) {
	return (duration_cast<duration<double>>(t2 - t1)).count();
}

void safe_rand_bytes(unsigned char *v, uint32_t n) {
	while (n > 0) {
		size_t round_size = n >= INT_MAX ? INT_MAX - 1 : n;
		RAND_bytes(v, round_size);
		v += round_size;
		n -= round_size;
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Specify the number of items for the test.\n";
		exit(1);
	}
	uint32_t N = atoi(argv[1]); // number of items

	// Generate N numbers to insert
	uint32_t *in_numbers = (uint32_t *)malloc(N * sizeof(uint32_t));
	if(!in_numbers) {
		std::cerr << "Malloc in_numbers failed.\n";
		exit(0);
	}
	safe_rand_bytes((unsigned char *)in_numbers, sizeof(*in_numbers) * N);

	// Generate N numbers different from in_numbers for succ queries
	uint32_t *out_numbers = (uint32_t *)malloc(N * sizeof(uint32_t));
	if(!out_numbers) {
		std::cerr << "Malloc out_numbers failed.\n";
		exit(0);
	}
	safe_rand_bytes((unsigned char *)out_numbers, sizeof(*out_numbers) * N);

	high_resolution_clock::time_point t1, t2;

	std::string testFlag = (argc > 2) ? argv[2] : ""; // flag for which benchmark(s) to run
	if (testFlag == "bst" || testFlag == "") {
		std::cout << "Testing Binary Search Tree..." << std::endl;
		// Create a bst using std::set
		std::set<uint32_t> bst;
		// Insert N items from in_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			bst.insert(in_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to insert " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";

		// Query N items from in_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			bst.find(in_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to query " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";

		// N Successor queries from out_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			bst.lower_bound(out_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to successor query " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";

		// Delete N items from bst
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			bst.erase(in_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to delete " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";
	} 
	
	if (testFlag == "veb" || testFlag == "") {
		std::cout << "Testing Van Emde Boas Tree..." << std::endl;
		VEB veb(UINT32_MAX); // |U| = 2^32
		
		// Insert N items from in_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			veb.Insert(in_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to Insert " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";

		// Query N items from in_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			veb.Query(in_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to query " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";

		// N Successor queries from out_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			veb.Successor(out_numbers[i]);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to successor query " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";
		
		// Delete N items from in_numbers
		t1 = high_resolution_clock::now();
		for (uint32_t i = 0; i < N; ++i) {
			veb.Delete(in_numbers[i]);
		}
		if (!veb.IsEmpty() || veb.Min() != 0 || veb.Max() != 0) {
			std::cerr << "delete failed to clear tree\n";
			exit(0);
		}
		t2 = high_resolution_clock::now();
		std::cout << "Time to delete " + std::to_string(N) + " items: " + std::to_string(elapsed(t1, t2)) + " secs\n";
	}

	return 0;
}
