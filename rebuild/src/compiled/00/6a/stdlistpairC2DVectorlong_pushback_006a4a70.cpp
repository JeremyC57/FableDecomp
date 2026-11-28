// Leaf: ?push_back@?$list@U?$pair@VC2DVector@@J@std@@V?$allocator@U?$pair@VC2DVector@@J@std@@@2@@std@@QAEXABU?$pair@VC2DVector@@J@2@@Z
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
typedef std::pair<C2DVector, long> Pulse;
template void std::list<Pulse>::push_back(const Pulse&);
