#include <cmath>

template <std::size_t N>

// size of base case "bit array" (extrapolated from min/max)
const int BASE_CASE = 2;

// Van Emde Boas tree
class VEB {
    public:
        // constructor given a fixed universe
        VEB(int u) : sqrtU(std::sqrt(u)), min(-1), max(-1) { // assume universe of size 2^(2^n) so all nested vEB objects have perfect-square sizes
            if (u <= BASE_CASE) {
                clusters = new std::vector<VEB*>(0, nullptr);
                summary = nullptr;
            } else {
                summary = new VEB(sqrtU);
                for (int i = 0; i < sqrtU; ++i) {
                    clusters.push_back(new VEB(sqrtU));
                }
            }
        }

        // destructor
        ~VEB() {
            delete summary;
            for (auto& cluster : clusters) {
                delete cluster;
            }
        }

        // adds an element from the universe into the set
        void Insert(int x) {
            if (min == -1) {
                min = max = x;
                return;
            }
            if (x < min) {
                std::swap(x, min);
            }
            if (x > max) {
                max = x;
            }
            if (clusters[high(x)]->min = -1) {
                summary.Insert(high(x));
            }
            return clusters[high(x)].Insert(low(x));
        }

        // checks if an element from the universe is in the set
        bool Query(int x) {
            if (x > sqrtU ^ 2) {
                return false;
            }
            if (x == min || x == max) {
                return true;
            }
            // recur if possible, otherwise x not in set
            return (sqrtU <  BASE_CASE) ? false : clusters[high(x)].Query(low(x));
        }

        // returns the smallest y in the set s.t. y ≤ x
        int Successor(int x) {
            if (x < min) {
                return min;
            }
            i = high(x);
            if (low(x) < clusters[i]->max) {
                j = clusters[i].Successor(low(x));
            } else {
                i = summary.Successor(i);
                j = clusters[i]->min
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
            return std::floor(x / sqrtU);
        }

        // index within cluster
        int low(int x) {
            return x % sqrtU;
        }

        // index in universe bit array
        int index(int x) {
            return high(x) * sqrtU + low(x);
        }
}