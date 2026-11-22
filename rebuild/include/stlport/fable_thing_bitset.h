#ifndef FABLE_STLPORT_THING_BITSET_H
#define FABLE_STLPORT_THING_BITSET_H
#include "stlport/fable_containers.h"

#ifdef FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
// The direct bitset<274>::test at 00410DE0 matches the original speed-built STLport body.
// Include before CThing and before the caller's row-head optimization.
template<> inline bool std::bitset<274>::test(size_t n) const
{
    if (n >= 274)
        std::__stl_throw_out_of_range("bitset");
    return _Unchecked_test(n);
}
#endif
#endif
