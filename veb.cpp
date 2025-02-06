#include "veb.h"
#include <vector>
#include <cmath>
#include <bitset>
#include <iostream>


/////////////////////////////
/////// Base Case VEB ///////
/////////////////////////////

VEB_Base::VEB_Base() : bitArray(0) {}

uint32_t VEB_Base::Min() {
    for (uint32_t i = 0; i < BASE_CASE; ++i) {
        if (bitArray.test(i)) {
            return i;
        }
    }
    return 0;
}

uint32_t VEB_Base::Max() {
    for (int i = BASE_CASE - 1; i >= 0; --i) { // int to prevent unsigned underflow
        if (bitArray.test(i)) {
            return i;
        }
    }
    return 0;
}

bool VEB_Base::IsEmpty() {
    return bitArray.none();
}

void VEB_Base::Insert(uint32_t x) {
    bitArray.set(x);
}

void VEB_Base::Delete(uint32_t x) {
    bitArray.reset(x);
}

bool VEB_Base::Query(uint32_t x) {
    return bitArray.test(x);
}

std::pair<bool, uint32_t> VEB_Base::Successor(uint32_t x) {
    for (uint32_t i = x; i < BASE_CASE; ++i) {
        if (bitArray.test(i)) {
            return {true, i};
        }
    }
    return {false, 0};
}


/////////////////////////////
/////// Recursive VEB ///////
/////////////////////////////

// constructor given a fixed universe
// assume universe of size 2^(2^n) so all nested VEB objects have perfect-square sizes
// also assume we will not construct the original VEB tree with base case size 256
VEB::VEB(uint32_t u) : min(0), max(0), isEmpty(true) {
    // sqrt(u), rounded up to the nearest power of 2
    // rounding necessary since initial universe is UINT32_MAX = 2^32 - 1, not 2^32; all recursive universes will be perfect square powers of 2
    sqrtU = std::exp2(std::ceil(std::log2(u)/2));
    
    summary = VEBFactory(sqrtU);
    clusters = std::vector<VEBInterface*>(sqrtU, nullptr);
}

VEB::~VEB() {
    delete summary;
    for (auto cluster : clusters) {
        delete cluster;
    }
}

uint32_t VEB::Min() {
    return min;
}

uint32_t VEB::Max() {
    return max;
}

bool VEB::IsEmpty() {
    return isEmpty;
}

// space-efficient approach with lazy-loading nested clusters, at the time cost of extra memory allocations during insertions
void VEB::Insert(uint32_t x) {
    if (isEmpty) {
        min = max = x;
        isEmpty = false;
        return;
    } else if (x == min || x == max) { // already inserted
        return;
    } else if (x < min ) {
        // lazy propogation of min: summary min is staggered one update behind this min
        std::swap(x, min);
    } else if (x > max) {
        max = x;
    }

    uint32_t i = high(x);
    if (clusters[i] == nullptr) {
        clusters[i] = VEBFactory(sqrtU);
        summary->Insert(i);
    }
    clusters[i]->Insert(low(x));
}

void VEB::Delete(uint32_t x) {
    if (isEmpty || x < min || x > max) { // item not there to delete
        return;
    }

    if (x == min) {
        if (summary->IsEmpty()) {
            min = max = 0;
            isEmpty = true;
            return;
        }
        uint32_t j = summary->Min();
        x = min = index(j, clusters[j]->Min());
    }

    uint32_t i = high(x);
    if (clusters[i] == nullptr) { // if x in a middle non-existant cluster
        return;
    }         
    clusters[i]->Delete(low(x));
    if (clusters[i] == nullptr ||  clusters[i]->IsEmpty()) { // if the above delete emptied the cluster
        summary->Delete(i);
        delete clusters[i];
        clusters[i] = nullptr;
    }

    if (x == max) {
        if (summary->IsEmpty()) {
            max = min;
        } else {
            uint32_t j = summary->Max();
            max = index(j, clusters[j]->Max());
        }
    }
}

bool VEB::Query(uint32_t x) {
    if (isEmpty) { // for highest-level empty tree
        return false;
    }
    if (x == min || x == max) {
        return true;
    }
    // recur if possible, otherwise x not in set
    if (clusters[high(x)] == nullptr) {
        return false;
    }
    return clusters[high(x)]->Query(low(x));
}

std::pair<bool, uint32_t> VEB::Successor(uint32_t x) {
    if (isEmpty) {
        return {false, 0};
    }
    if (x <= min) {
        return {true, min};
    }

    uint32_t i = high(x);
    uint32_t j;
    if (clusters[i] != nullptr && low(x) <= clusters[i]->Max()) { // successor in same cluster
        auto succ = clusters[i]->Successor(low(x));
        // successor is known to exist, no need to check
        j = succ.second;
    } else { // no successor in this cluster
        if (i >= summary->Max()) { // no next nonempty clusters
            return {false, 0};
        }
        auto succ = summary->Successor(i+1); // find next nonempty cluster
        if (!succ.first) {
            return {false, 0};
        }
        // successor is known to exist, no need to check
        i = succ.second;
        j = clusters[i]->Min();
    }
    return {true, index(i, j)};
}

uint32_t VEB::high(uint32_t x) {
    return std::floor(x / sqrtU);
}

// index within cluster
uint32_t VEB::low(uint32_t x) {
    return x % sqrtU;
}

// index in universe bit array (ith cluster, jth index in cluster)
uint32_t VEB::index(uint32_t i, uint32_t j) {
    return i * sqrtU + j;
}

VEBInterface* VEB::VEBFactory(uint32_t u) {
    if (u <= BASE_CASE) {
        return new VEB_Base();
    } else {
        return new VEB(u);
    }
}
