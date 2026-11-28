// Leaf: ?clear@?$_List_base@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXXZ
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
template void std::_List_base<C3DVector, std::allocator<C3DVector> >::clear();
