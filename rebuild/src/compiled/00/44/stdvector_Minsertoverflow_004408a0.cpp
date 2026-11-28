// 004408a0 std::vector<float>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@MV?$allocator@M@std@@@std@@IAEXPAMABMABU__true_type@2@I_N@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"

namespace std {
template void vector<float>::_M_insert_overflow(float*, const float&, const __true_type&, unsigned int, bool);
}
