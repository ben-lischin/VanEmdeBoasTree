#include "VEB.h"
#include <iostream>
#include <cassert>

int main() {
    VEB veb(UINT32_MAX);

    assert(veb.Query(0) == false);
    assert(veb.Successor(0).first == 0);
    assert(veb.Query(65535) == false);
    assert(veb.Successor(65535).first == 0);

    assert(veb.Query(1) == false);
    assert(veb.Query(6) == false);
    assert(veb.Query(7) == false);
    assert(veb.Query(100) == false);
    assert(veb.Query(1000) == false);
    assert(veb.Query(5000) == false);
    assert(veb.Query(10000) == false);
    assert(veb.Query(30000) == false);
    assert(veb.Query(55000) == false);
    veb.Insert(1);
    veb.Insert(6);
    veb.Insert(7);
    veb.Insert(100);
    veb.Insert(1000);
    veb.Insert(5000);
    veb.Insert(10000);
    veb.Insert(30000);
    veb.Insert(55000);
    assert(veb.Query(1) == true);
    assert(veb.Query(6) == true);
    assert(veb.Query(7) == true);
    assert(veb.Query(100) == true);
    assert(veb.Query(1000) == true);
    assert(veb.Query(5000) == true);
    assert(veb.Query(10000) == true);
    assert(veb.Query(30000) == true);
    assert(veb.Query(55000) == true);

    assert(veb.Query(0) == false);
    assert(veb.Query(8) == false);
    assert(veb.Query(9) == false);
    assert(veb.Query(11) == false);
    assert(veb.Query(1413772034) == false);

    assert(veb.Successor(0).second == 1);
    assert(veb.Successor(1).second == 1);
    assert(veb.Successor(2).second == 6);
    assert(veb.Successor(3).second == 6);
    assert(veb.Successor(6).second == 6);
    assert(veb.Successor(7).second == 7);
    assert(veb.Successor(10).second == 100);
    assert(veb.Successor(64).second == 100);
    assert(veb.Successor(99).second == 100);
    assert(veb.Successor(100).second == 100);
    assert(veb.Successor(101).second == 1000);
    assert(veb.Successor(504).second == 1000);
    assert(veb.Successor(999).second == 1000);
    assert(veb.Successor(1000).second == 1000);
    assert(veb.Successor(1400).second == 5000);
    assert(veb.Successor(7890).second == 10000);
    assert(veb.Successor(10100).second == 30000);
    assert(veb.Successor(54999).second == 55000);
    assert(veb.Successor(55000).second == 55000);
    assert(veb.Successor(55001).first == 0);

    veb.Query(0);
    assert(veb.Query(0) == false);
    veb.Insert(0);
    assert(veb.Query(0) == true);

    assert(veb.Query(65535) == false);
    assert(veb.Successor(65535).first == 0);
    veb.Insert(65535);    
    assert(veb.Query(65535) == true);
    assert(veb.Successor(65535).second == 65535);

    assert(veb.Query(1048577) == false);
    assert(veb.Query(23148641) == false);
    assert(veb.Query(89148379) == false);
    assert(veb.Query(536870913) == false);
    veb.Insert(1048577);
    veb.Insert(23148641);
    veb.Insert(89148379);
    veb.Insert(536870913);
    assert(veb.Query(1048577) == true);
    assert(veb.Query(23148641) == true);
    assert(veb.Query(89148379) == true);
    assert(veb.Query(536870913) == true);
    

    assert(veb.Successor(65536).second == 1048577);
    assert(veb.Successor(123414).second == 1048577);
    assert(veb.Successor(1048577).second == 1048577);
    assert(veb.Successor(1048578).second == 23148641);
    assert(veb.Successor(1941937).second == 23148641);
    assert(veb.Successor(23148641).second == 23148641);
    assert(veb.Successor(23148642).second == 89148379);
    assert(veb.Successor(87019104).second == 89148379);
    assert(veb.Successor(89148379).second == 89148379);
    assert(veb.Successor(89148380).second == 536870913);
    assert(veb.Successor(101876183).second == 536870913);
    assert(veb.Successor(536870914).first == 0);

    return 0;
}