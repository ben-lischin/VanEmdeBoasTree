#ifndef VEB_HPP
#define VEB_HPP

#include <vector>
#include <cmath>
#include <bitset>


// size of base case bitset
const uint32_t BASE_CASE = 256;

class VEBInterface {
    public:
        virtual ~VEBInterface() {}
        // adds an element from the universe into the tree
        virtual void Insert(uint32_t x) = 0;
        // removes an element in the universe from the tree
        virtual void Delete(uint32_t x) = 0;
        // checks if an element from the universe is in the tree
        virtual bool Query(uint32_t x) = 0;
        // finds the smallest y in the tree s.t. y ≥ x, if any
        virtual std::pair<bool, uint32_t> Successor(uint32_t x) = 0;
        // returns the minimum element in the tree
        virtual uint32_t Min() = 0;
        // returns the maximum element in the tree
        virtual uint32_t Max() = 0;
        // returns if there are no elements in the tree
        virtual bool IsEmpty() = 0;
};

// with a base case universe size, stop recurring and linearly search/manipulate a bitset
class VEB_Base : public VEBInterface {
    public:
        VEB_Base();
        uint32_t Min() override;
        uint32_t Max() override;
        bool IsEmpty() override;
        void Insert(uint32_t x) override;
        void Delete(uint32_t x) override;
        bool Query(uint32_t x) override;
        std::pair<bool, uint32_t> Successor(uint32_t x) override;

    private:
        // base case bit array
        std::bitset<BASE_CASE> bitArray;
        // avoid extra space of min/max/isEmpty fields, since the universe is small enough to quickly search. This also avoids extra operation to update them

};

// recursive Van Emde Boas tree
class VEB : public VEBInterface {
    public:
        VEB(uint32_t u);
        ~VEB();
        uint32_t Min() override;
        uint32_t Max() override;
        bool IsEmpty() override;
        void Insert(uint32_t x) override;
        void Delete(uint32_t x) override;
        bool Query(uint32_t x) override;
        std::pair<bool, uint32_t> Successor(uint32_t x) override;

    private:
        // number of next-level clusters; size of next-level clustrs
        // reduce expensive square root calculations by performing it once in the constructor and reusing that value
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
        bool isEmpty;

        // cluster index
        uint32_t high(uint32_t x);
        // index within cluster
        uint32_t low(uint32_t x);
        // index in universe bit array (ith cluster, jth index in cluster)
        uint32_t index(uint32_t i, uint32_t j);
        // produces the next-level VEB object (base or recursive)
        VEBInterface* VEBFactory(uint32_t u);
};

#endif
