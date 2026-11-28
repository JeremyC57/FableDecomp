// Leaf: ?clear@?$_List_base@U?$pair@VC2DVector@@J@std@@V?$allocator@U?$pair@VC2DVector@@J@std@@@2@@std@@QAEXXZ
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
typedef std::pair<C2DVector, long> Pulse;
template void std::_List_base<Pulse, std::allocator<Pulse> >::clear();
