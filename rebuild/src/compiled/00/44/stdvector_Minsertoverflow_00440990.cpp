// Flags: 
// 00440990 std::vector<long>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z
#include "stlport/fable_containers.h"

template void std::vector<long>::_M_insert_overflow(long*, const long&,
    const std::__true_type&, unsigned int, bool);
