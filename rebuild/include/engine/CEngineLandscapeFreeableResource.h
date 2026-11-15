#ifndef FABLE_ENGINE_CENGINELANDSCAPEFREEABLERESOURCE_H
#define FABLE_ENGINE_CENGINELANDSCAPEFREEABLERESOURCE_H
#include "engine/CMovableResource.h"
// Retail RTTI: CEngineLandscapeFreeableResource : CMovableResource.
// Release base is one vptr: tree ctor00BF6E20 starts DynamicBuildTemplate at4,
// GPUUpdateTask at8 and CLODLoadInfo atC; all subsequent native members agree.
// Both debug donors add LRUPrev/LRUNext at4/8. These debug-only links are absent
// in this release ABI; retaining them would shift every tree member by8.
class CEngineLandscapeFreeableResource : public CMovableResource {
public:
    virtual bool FreeableResourceFreeBlock() = 0; // original PDB slot16
};
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeFreeableResource) == 4);
#endif
