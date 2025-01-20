#ifndef VEB_H
#define VEB_H

#include <vector>
#include <cmath>
#include <iostream>


// size of base case "bit array" (extrapolated from min/max)
const uint32_t BASE_CASE = 2;

// Van Emde Boas tree
class VEB {
    public:
        // constructor given a fixed universe
        VEB(uint32_t u) : u(u), min(0), max(0) { // assume universe of size 2^(2^n) so all nested vEB objects have perfect-square sizes
            if (u <= BASE_CASE) {
                summary = nullptr;
                clusters = std::vector<VEB*>(0, nullptr);
            } else {
                // // sqrt(u), rounded up to the nearest power of 2
                // clusterCount = static_cast<uint32_t>(exp2(std::ceil(log2(u)/2)));
                // // sqrt(u), rounded down to the nearest power of 2
                // clusterSize = static_cast<uint32_t>(exp2(std::floor(log2(u)/2)));
                clusterCount = clusterSize = std::sqrt(u);
                // allocate memory for recursion
                summary = new VEB(clusterSize);
                clusters = std::vector<VEB*>(clusterCount, nullptr);
                // for (uint32_t i = 0; i < clusterCount; ++i) { // might have to remove here and do lazy loading as we recur in the methods? (just insert?)
                //     clusters[i] = new VEB(clusterSize);
                // }
            }
        }

        // destructor
        ~VEB() {
            delete summary;
            for (auto& cluster : clusters) {
                delete cluster;
            }
        }

        uint32_t Max() {
            return max;
        }

        uint32_t Min() {
            return min;
        }

        // adds an element from the universe into the set
        void Insert(uint32_t x) {
            if (min == 0 && max == 0) {
                min = max = x;
            }
            max = std::max(x, max);
            min = std::min(x, min);
            
            if (u <= BASE_CASE) {
                return;
            }
            if (clusters[high(x)] == nullptr) { 
                summary->Insert(high(x));
                clusters[high(x)] = new VEB(clusterSize); // lazy loading of nested clusters, as they need to be entered
            }
            clusters[high(x)]->Insert(low(x));
        }

        // checks if an element from the universe is in the set
        bool Query(uint32_t x) {
            if (x == min || x == max) {
                return true;
            }
            // recur if possible, otherwise x not in set
            // return (u  <=  BASE_CASE) ? false : clusters[high(x)]->Query(low(x));
            if (u <= BASE_CASE || clusters[high(x)] == nullptr) {
                return false;
            }
            return clusters[high(x)]->Query(low(x));
        }

        // returns the smallest y in the set s.t. y ≤ x
        std::pair<bool, uint32_t> Successor(uint32_t x) {
            // if (u <= BASE_CASE) {
            //     if (x < min) {
            //         return {true, min};
            //     } else {
            //         return {false, 0};
            //     }
            // }
            if (x <= min) {
                return {true, min};
            }
            if (x > max) {
                return {false, 0};
            }
            uint32_t i = high(x);
            uint32_t j;
            if (clusters[i] != nullptr && low(x) <= clusters[i]->Max()) { // successor in same cluster
                auto succ = clusters[i]->Successor(low(x));
                j = succ.second;
            } else { // no successor in this cluster
                auto succ = summary->Successor(i); // find next nonempty cluster
                if (!succ.first) { // is this necessary?
                    return {false, 0};
                }
                i = succ.second;
                j = clusters[i]->Min();
            }
            return {true, index(i, j)};

        }

    private:
        // current-level universe size
        uint32_t u;
        // number of next-level clusters
        uint32_t clusterCount;
        // size of next-level clusters
        uint32_t clusterSize;
        // nested clusters
        std::vector<VEB*> clusters;
        // highest-level summary struct, 
        VEB* summary;
        // minimum occupied index in the cluster
        uint32_t min;
        // maximum occupied index in the cluster
        uint32_t max;

        // cluster index
        uint32_t high(uint32_t x) {
            return std::floor(x / clusterSize);
        }

        // index within cluster
        uint32_t low(uint32_t x) {
            return x % clusterSize;
        }

        // index in universe bit array (ith cluster, jth index in cluster)
        uint32_t index(uint32_t i, uint32_t j) {
            return i * clusterSize + j;
        }
        


};

#endif