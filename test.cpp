// Author: Prashant Pandey <prashant.pandey@utah.edu>
// For use in CS6968 & CS5968

#include <iostream>
#include <set>
#include <chrono>
#include <openssl/rand.h>
#include "veb.hpp"
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

	std::set<uint32_t> bst;
	VEB veb(UINT32_MAX); // |U| = 2^32

	for (uint32_t i = 0; i < N; ++i) {
		bst.insert(in_numbers[i]);
		veb.Insert(in_numbers[i]);
	}
	for (uint32_t i = 0; i < N; ++i) {
		auto ret1 = bst.find(out_numbers[i]);
		auto ret2 = veb.Query(out_numbers[i]);
		if ((ret1 != bst.end() && !ret2) || (ret1 == bst.end() && ret2)) {
			std::cout << "Query failed for item: " << out_numbers[i] << std::endl;
		}
		auto ret3 = veb.Query(in_numbers[i]);
		if (!ret3) {
			std::cout << "Query returned false for inserted item: " << in_numbers[i] << std::endl;
		}
	}
	for (uint32_t i = 0; i < N; ++i) {
		auto ret1 = bst.lower_bound(out_numbers[i]);
		auto ret2 = veb.Successor(out_numbers[i]);
		if ((ret1 == bst.end() && ret2.first == 1) || (ret1 != bst.end() && ret2.first == 0) || (ret1 != bst.end() && ret2.first == 1 && *ret1 != ret2.second)) {
			std::cout << "Successor failed for item: " << out_numbers[i] << std::endl;
		}
	}
	for (uint32_t i = 0; i < N; ++i) {
		bst.erase(in_numbers[i]);
		veb.Delete(in_numbers[i]);
	}
	if (!veb.IsEmpty() || veb.Min() != 0 || veb.Max() != 0) {
		std::cout << "Delete failed to clear tree" << std::endl;
	}

	return 0;
}
