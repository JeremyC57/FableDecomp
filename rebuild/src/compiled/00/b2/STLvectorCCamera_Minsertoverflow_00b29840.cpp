// Leaf: ?_M_insert_overflow@?$vector@VCCamera@@V?$allocator@VCCamera@@@std@@@std@@IAEXPAVCCamera@@ABV3@ABU__false_type@2@I_N@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CCamera.h"
#include "stlport/fable_containers.h"

template void std::vector<CCamera>::_M_insert_overflow(CCamera*, const CCamera&, const std::__false_type&, unsigned int, bool);
