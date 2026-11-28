// Leaf: ?push_back@?$list@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXABVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
template void std::list<C3DVector>::push_back(const C3DVector&);
