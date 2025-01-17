#include <cmath>

template <std::size_t N>

// size of base case bit array
const int BASE_CASE = 16;

// Van Emde Boas tree
class VEB {
    public:
        // constructor given a fixed universe
        VEB(int u) : sqrtU(std::sqrt(x)), min(-1), max(-1) { // assume universe size is a perfect square (i.e. even power of 2)
            if (u <= BASE_CASE) {
                // bit array in memory?
            } else {
                this->summary = new VEB(this->sqrtU);
                for (int i = 0; i < this->sqrtU; ++i) {
                    clusters.push_back(new VEB(this->sqrtU));
                }
            }
        }

        // destructor
        ~VEB() {
            delete this->summary;
            for (auto cluster : this->clusters) {
                delete cluster; // pointer vs object?
            }
        }

        // adds an element from the universe into the set
        void Insert(int x) {
            if (this->min == -1) {
                this->min = this->max = x;
                return;
            }
            if (x < this->min) {
                std::swap(x,this->min);
            }
            if (x > this->max) {
                this->max = x;
            }
            if (this->clusters[high(x)]->min = -1) {
                this->summary.Insert(high(x));
            }
            this->clusters[high(x)].Insert(low(x));
        }

        // checks if an element from the universe is in the set
        bool Query(int x) {

        }

        // returns the smallest y in the set s.t. y ≤ x
        int Successor(int x) {
            if (x < this->min) {
                return this->min;
            }
            i = high(x);
            if (low(x) < this->clusters[i]->max) {
                j = this->clusters[i].Successor(low(x));
            } else {
                i = this->summary.Successor(i);
                j = this->clusters[i]->min
            }
            return index(i, j);
        }

    private:
        // next-level cluster size, also number of next-level clusters
        const int sqrtU;
        // nested clusters
        std::vector<VEB*> clusters;
        // highest-level summary struct, 
        VEB* summary;
        // minimum occupied index in the cluster
        int min;
        // maximum occupied index in the cluster
        int max;

        // cluster index
        int high(int x) {
            return std::floor(x / this->sqrtU);
        }
        // index within cluster
        int low(int x) {
            return x % this->sqrtU;
        }
        // index in universe bit array
        int index(int x) {
            return high(x) * this->sqrtU + low(x);
        }
}