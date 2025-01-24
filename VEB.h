#ifndef VEB_H
#define VEB_H

#include <vector>
#include <cmath>
#include <bitset>


// size of base case bitset
const uint32_t BASE_CASE = 16;

class VEBInterface {
    public:
        virtual ~VEBInterface() {}
        virtual void Insert(uint32_t x) = 0;
        virtual bool Query(uint32_t x) = 0;
        virtual std::pair<bool, uint32_t> Successor(uint32_t x) = 0;
        virtual uint32_t Min() = 0;
        virtual uint32_t Max() = 0;
};

// with a base case universe size of 16, stop recurring and linearly search/manipulate a bitset
class VEB_Base : public VEBInterface {
    public:
        VEB_Base() : bitArray(0), min(0), max(0), isEmpty(true) {}

        uint32_t Min() override {
            return min;
        }

        uint32_t Max() override {
            return max;
        }

        void Insert(uint32_t x) override {
            bitArray.set(x);
            if (isEmpty) {
                min = max = x;
                isEmpty = false;
                return;
            }
            if (x < min) {
                min = x;
            } else if (x > max) {
                max = x;
            }
            // or should we not maintain a min/max and just loop??
        }

        bool Query(uint32_t x) override {
            return bitArray.test(x);
        }

        std::pair<bool, uint32_t> Successor(uint32_t x) override {
            if (x > max)  {
                return {false, 0};
            }
            for (uint32_t i = x; i < 16; ++i) {
                if (bitArray.test(i)) {
                    return {true, i};
                }
            }
            return {false, 0};
        }

    private:
        // base case bit array
        std::bitset<BASE_CASE> bitArray;
        uint32_t min;
        uint32_t max;
        bool isEmpty;

};

// recursive Van Emde Boas tree
class VEB : public VEBInterface {
    public:
        // constructor given a fixed universe
        // assume universe of size 2^(2^n) so all nested vEB objects have perfect-square sizes
        // also assume we will not construct the original vEB tree with base case size 16
        VEB(uint32_t u) : u(u), min(0), max(0), isEmpty(true) {
            // sqrt(u), rounded up to the nearest power of 2
            // rounding necessary since initial universe is UINT32_MAX = 2^32 - 1, not 2^32; all recursive universes will be perfect square powers of 2
            sqrtU = std::exp2(std::ceil(std::log2(u)/2));
            
            summary = VEBFactory(sqrtU);
            clusters = std::vector<VEBInterface*>(sqrtU, nullptr);
        }

        // destructor
        ~VEB() {
            delete summary;
            for (auto& cluster : clusters) {
                delete cluster;
            }
        }

        uint32_t Min() {
            return min;
        }

        uint32_t Max() {
            return max;
        }

        // adds an element from the universe into the set
        // space-efficient approach with lazy-loading nested clusters, at the time cost of extra heap allocations during insertions
        void Insert(uint32_t x) {
            if (isEmpty) {
                min = max = x;
                isEmpty = false;
            } else if (x == min || x == max) { // already inserted
                return;
            } else if (x < min ) {
                min = x;
            } else if (x > max) {
                max = x;
            }

            uint32_t i = high(x);
            if (clusters[i] == nullptr) { 
                summary->Insert(i);
                clusters[i] = VEBFactory(sqrtU); // lazy loading of nested clusters, as they need to be entered
            }
            clusters[i]->Insert(low(x));
        }

        // // removes an element in the universe from the set
        // void Delete(uint32_t x) {
        //     if (isEmpty || (!isEmpty && (x < min || x > max))) { // nothing to delete
        //         return;
        //     }
        //     if (x == min && x == max) { // x is only item in this cluster
        //         isEmpty = true;
        //         min = max = 0;
        //         return;
        //         // can also make the clusters and summary nullptrs like reinforcing lazy loading
        //         // is it possible to make this cluster itself nullptr in the parent cluster?
        //     }
        //     if (x == min) {
        //         // min = next smallest
                
        //     }
        //     if (x == max) {
        //         // max = next largest
        //     }
        //     // x within some nested cluster
        //     clusters[high(x)]->Delete(low(x));
        // }

        // checks if an element from the universe is in the set
        bool Query(uint32_t x) {
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

        // returns the smallest y in the set s.t. y ≤ x
        std::pair<bool, uint32_t> Successor(uint32_t x) {
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
                // successor is known to exist, no need to check
                i = succ.second;
                j = clusters[i]->Min();
            }
            return {true, index(i, j)};
        }

    private:
        // current-level universe size
        uint32_t u;
        // number of next-level clusters; size of next-level clustrs
        uint32_t sqrtU;
        // nested clusters
        std::vector<VEBInterface*> clusters;
        // highest-level summary struct, 
        VEBInterface* summary;
        // minimum occupied index in the cluster
        uint32_t min;
        // maximum occupied index in the cluster
        uint32_t max;
        // if the tree is empty
        bool isEmpty; // only really useful for the case of query(0)/successor(0) on an empty tree (first-level) to avoid confusion with the default min/max of 0; nested empty trees are null

        // cluster index
        uint32_t high(uint32_t x) {
            return std::floor(x / sqrtU);
        }

        // index within cluster
        uint32_t low(uint32_t x) {
            return x % sqrtU;
        }

        // index in universe bit array (ith cluster, jth index in cluster)
        uint32_t index(uint32_t i, uint32_t j) {
            return i * sqrtU + j;
        }

        VEBInterface* VEBFactory(uint32_t u) {
            if (u <= BASE_CASE) {
                return new VEB_Base();
            } else {
                return new VEB(u);
            }
        }
};

#endif