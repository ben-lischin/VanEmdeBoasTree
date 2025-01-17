#include <cmath>

// Van Emde Boas tree
class VEB {
    public:
        // constructor given a fixed universe
        VEB(int u) {

        }
        // destructor
        ~VEB() {

        }
        // adds an element from the universe into the set
        void Insert(int x) {

        }
        // checks if an element from the universe is in the set
        bool Query(int x) {

        }
        // returns the smallest y in the set s.t. y ≤ x
        int Successor(int x) {

        }

    private:
        // next-level cluster size, also number of next-level clusters
        const int sqrtU;
        // nested clusters
        std::vector<VEB*> clusters;
        // highest-level summary struct, 
        std::bitset<sqrtU> summary
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