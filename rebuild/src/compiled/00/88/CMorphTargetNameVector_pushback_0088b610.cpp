// 0088b610 std::vector<C3DSkeletalBoneMorph>::push_back
// Flags: /Gr
// Leaf: ?push_back@?$vector@VC3DSkeletalBoneMorph@@V?$allocator@VC3DSkeletalBoneMorph@@@std@@@std@@QAEXABVC3DSkeletalBoneMorph@@@Z
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "fable_string.h"
#include "engine/C3DSkeletalMorph.h"

template void std::vector<C3DSkeletalBoneMorph>::push_back(const C3DSkeletalBoneMorph&);
