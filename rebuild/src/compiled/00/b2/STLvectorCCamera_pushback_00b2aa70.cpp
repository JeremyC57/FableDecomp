// Leaf: ?push_back@?$vector@VCCamera@@V?$allocator@VCCamera@@@std@@@std@@QAEXABVCCamera@@@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CCamera.h"
#include "stlport/fable_containers.h"

template void std::vector<CCamera>::push_back(const CCamera&);
